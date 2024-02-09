#include "RNOH/TextMeasurer.h"
#include "RNOH/ArkJS.h"
#include <native_drawing/drawing_text_typography.h>
#include <native_drawing/drawing_font_collection.h>

OH_Drawing_FontWeight mapValueToFontWeight(int value) {
    switch (value) {
    case 100:
        return OH_Drawing_FontWeight::FONT_WEIGHT_100;
    case 200:
        return OH_Drawing_FontWeight::FONT_WEIGHT_200;
    case 300:
        return OH_Drawing_FontWeight::FONT_WEIGHT_300;
    case 400:
        return OH_Drawing_FontWeight::FONT_WEIGHT_400;
    case 500:
        return OH_Drawing_FontWeight::FONT_WEIGHT_500;
    case 600:
        return OH_Drawing_FontWeight::FONT_WEIGHT_600;
    case 700:
        return OH_Drawing_FontWeight::FONT_WEIGHT_700;
    case 800:
        return OH_Drawing_FontWeight::FONT_WEIGHT_800;
    case 900:
        return OH_Drawing_FontWeight::FONT_WEIGHT_900;
    default:
        return OH_Drawing_FontWeight::FONT_WEIGHT_400;
    }
}

using namespace facebook;
using namespace rnoh;

react::TextMeasurement TextMeasurer::measure(react::AttributedString attributedString,
                                             react::ParagraphAttributes paragraphAttributes,
                                             react::LayoutConstraints layoutConstraints) {
    react::TextMeasurement result = {{0, 0}, {}};
    auto fragments = attributedString.getFragments();
    auto canUseOHOSTextMeasurer = fragments.size() == 1 && !fragments[0].isAttachment() &&
                                  isnan(fragments[0].textAttributes.letterSpacing) &&
                                  isnan(fragments[0].textAttributes.lineHeight);
    auto isNDKTextMeasuringEnabled = this->m_featureFlagRegistry->getFeatureFlagStatus("ENABLE_NDK_TEXT_MEASURING");
    if (canUseOHOSTextMeasurer || isNDKTextMeasuringEnabled) {
        auto fragments = attributedString.getFragments();
        auto typographyStyle = OH_Drawing_CreateTypographyStyle();
        if (paragraphAttributes.maximumNumberOfLines) {
            OH_Drawing_SetTypographyTextMaxLines(typographyStyle, paragraphAttributes.maximumNumberOfLines);
        }
        auto typographyHandler =
            OH_Drawing_CreateTypographyHandler(typographyStyle, OH_Drawing_CreateFontCollection());
        // have to record attachment count manually
        auto attachmentNums = 0;
        double fontHeight = 0.0;
        for (auto fragment : fragments) {
            // for nested texts
            if (!fragment.isAttachment()) {
                auto textStyle = OH_Drawing_CreateTextStyle();
                // set fontSize to default for negative values(same as iOS)
                if (fragment.textAttributes.fontSize <= 0) {
                    fragment.textAttributes.fontSize = 14;
                }
                OH_Drawing_SetTextStyleFontSize(textStyle, fragment.textAttributes.fontSize);
                // new NDK for setting letterSpacing
                if (!isnan(fragment.textAttributes.letterSpacing)) {
                    OH_Drawing_SetTextStyleLetterSpacing(textStyle, fragment.textAttributes.letterSpacing);
                }
                if (!isnan(fragment.textAttributes.lineHeight)) {
                    // fontSize * fontHeight = lineHeight + 1, no direct ndk for setting lineHeight so do it
                    // in this weird way
                    fontHeight = (fragment.textAttributes.lineHeight - 1) / fragment.textAttributes.fontSize;
                    OH_Drawing_SetTextStyleFontHeight(textStyle, fontHeight);
                }
                if (fragment.textAttributes.fontWeight.has_value()) {
                    OH_Drawing_SetTextStyleFontWeight(
                        textStyle, mapValueToFontWeight(int(fragment.textAttributes.fontWeight.value())));
                }
                if (!fragment.textAttributes.fontFamily.empty()) {
                    const char *fontFamilies[] = {fragment.textAttributes.fontFamily.c_str()};
                    OH_Drawing_SetTextStyleFontFamilies(textStyle, 1, fontFamilies);
                }
                // push text and corresponding textStyle to handler
                OH_Drawing_TypographyHandlerPushTextStyle(typographyHandler, textStyle);
                OH_Drawing_TypographyHandlerAddText(typographyHandler, fragment.string.c_str());
                OH_Drawing_DestroyTextStyle(textStyle);
            } else {
                // using placeholder span for inline views
                OH_Drawing_PlaceholderSpan inlineView = {
                    fragment.parentShadowView.layoutMetrics.frame.size.width,
                    fragment.parentShadowView.layoutMetrics.frame.size.height,
                    // align to the line's baseline
                    ALIGNMENT_ABOVE_BASELINE,
                    TEXT_BASELINE_ALPHABETIC,
                    0,
                };
                // push placeholder to handler
                OH_Drawing_TypographyHandlerAddPlaceholder(typographyHandler, &inlineView);
                attachmentNums++;
            }
        }
        auto typography = OH_Drawing_CreateTypography(typographyHandler);
        OH_Drawing_TypographyLayout(typography, layoutConstraints.maximumSize.width
                                                    ? layoutConstraints.maximumSize.width
                                                    : std::numeric_limits<facebook::react::Float>::max());
        auto attachments = OH_Drawing_TypographyGetRectsForPlaceholders(typography);
        // calculate attachment sizes and positions
        for (auto i = 0; i < attachmentNums; i++) {
            react::TextMeasurement::Attachment attachment;
            attachment.frame.origin.x = OH_Drawing_GetLeftFromTextBox(attachments, i);
            attachment.frame.origin.y = OH_Drawing_GetTopFromTextBox(attachments, i);
            attachment.frame.size.width =
                OH_Drawing_GetRightFromTextBox(attachments, i) - OH_Drawing_GetLeftFromTextBox(attachments, i);
            attachment.frame.size.height =
                OH_Drawing_GetBottomFromTextBox(attachments, i) - OH_Drawing_GetTopFromTextBox(attachments, i);
            result.attachments.push_back(std::move(attachment));
        }
        auto height = OH_Drawing_TypographyGetHeight(typography);
        auto longestLineWidth = OH_Drawing_TypographyGetLongestLine(typography);
        result.size.width = longestLineWidth + 0.5;
        result.size.height = height;
        OH_Drawing_DestroyTypography(typography);
        OH_Drawing_DestroyTypographyHandler(typographyHandler);
        OH_Drawing_DestroyTypographyStyle(typographyStyle);
    } else {
        m_taskExecutor->runSyncTask(TaskThread::MAIN, [&result, measureTextRef = m_measureTextFnRef, env = m_env,
                                                       &attributedString, &paragraphAttributes, &layoutConstraints]() {
            ArkJS arkJs(env);
            auto napiMeasureText = arkJs.getReferenceValue(measureTextRef);
            auto napiAttributedStringBuilder = arkJs.createObjectBuilder();
            napiAttributedStringBuilder.addProperty("string", attributedString.getString());
            std::vector<napi_value> napiFragments = {};
            for (auto fragment : attributedString.getFragments()) {
                auto textAttributesBuilder = arkJs.createObjectBuilder();
                textAttributesBuilder.addProperty("fontSize", fragment.textAttributes.fontSize);
                textAttributesBuilder.addProperty("lineHeight", fragment.textAttributes.lineHeight);
                if (!fragment.textAttributes.fontFamily.empty()) {
                    textAttributesBuilder.addProperty("fontFamily", fragment.textAttributes.fontFamily);
                }
                textAttributesBuilder.addProperty("letterSpacing", fragment.textAttributes.letterSpacing);
                if (fragment.textAttributes.fontWeight.has_value()) {
                    textAttributesBuilder.addProperty("fontWeight", int(fragment.textAttributes.fontWeight.value()));
                }

                auto napiFragmentBuilder = arkJs.createObjectBuilder();
                napiFragmentBuilder.addProperty("string", fragment.string)
                    .addProperty("textAttributes", textAttributesBuilder.build());
                if (fragment.isAttachment())
                    napiFragmentBuilder.addProperty(
                        "parentShadowView",
                        arkJs.createObjectBuilder()
                            .addProperty("tag", fragment.parentShadowView.tag)
                            .addProperty(
                                "layoutMetrics",
                                arkJs.createObjectBuilder()
                                    .addProperty(
                                        "frame",
                                        arkJs.createObjectBuilder()
                                            .addProperty(
                                                "size", arkJs.createObjectBuilder()
                                                            .addProperty("width", fragment.parentShadowView
                                                                                      .layoutMetrics.frame.size.width)
                                                            .addProperty("height", fragment.parentShadowView
                                                                                       .layoutMetrics.frame.size.height)
                                                            .build())
                                            .build())
                                    .build())
                            .build());

                napiFragments.push_back(napiFragmentBuilder.build());
            }
            napiAttributedStringBuilder.addProperty("fragments", arkJs.createArray(napiFragments));

            auto napiParagraphAttributesBuilder = arkJs.createObjectBuilder();
            napiParagraphAttributesBuilder.addProperty("maximumNumberOfLines",
                                                       paragraphAttributes.maximumNumberOfLines);

            auto napiLayoutConstraintsBuilder = arkJs.createObjectBuilder();
            napiLayoutConstraintsBuilder.addProperty("maximumSize",
                                                     arkJs.createObjectBuilder()
                                                         .addProperty("width", layoutConstraints.maximumSize.width)
                                                         .addProperty("height", layoutConstraints.maximumSize.height)
                                                         .build());

            auto resultNapiValue = arkJs.call(napiMeasureText, {napiAttributedStringBuilder.build(),
                                                                napiParagraphAttributesBuilder.build(),
                                                                napiLayoutConstraintsBuilder.build()});

            result.size.width =
                arkJs.getDouble(arkJs.getObjectProperty(arkJs.getObjectProperty(resultNapiValue, "size"), "width"));
            result.size.height =
                arkJs.getDouble(arkJs.getObjectProperty(arkJs.getObjectProperty(resultNapiValue, "size"), "height"));
            auto napiAttachments = arkJs.getObjectProperty(resultNapiValue, "attachmentLayouts");
            for (auto i = 0; i < arkJs.getArrayLength(napiAttachments); i++) {
                auto napiAttachment = arkJs.getArrayElement(napiAttachments, i);
                auto napiPositionRelativeToContainer =
                    arkJs.getObjectProperty(napiAttachment, "positionRelativeToContainer");
                auto napiSize = arkJs.getObjectProperty(napiAttachment, "size");
                react::TextMeasurement::Attachment attachment;
                attachment.frame.origin.x =
                    arkJs.getDouble(arkJs.getObjectProperty(napiPositionRelativeToContainer, "x"));
                attachment.frame.origin.y =
                    arkJs.getDouble(arkJs.getObjectProperty(napiPositionRelativeToContainer, "y"));
                attachment.frame.size.width = arkJs.getDouble(arkJs.getObjectProperty(napiSize, "width"));
                attachment.frame.size.height = arkJs.getDouble(arkJs.getObjectProperty(napiSize, "height"));
                result.attachments.push_back(std::move(attachment));
            }
        });
    }

    return result;
}