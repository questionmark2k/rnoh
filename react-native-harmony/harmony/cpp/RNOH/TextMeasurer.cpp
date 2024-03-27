#include "RNOH/TextMeasurer.h"
#include "RNOH/ArkJS.h"
#include <native_drawing/drawing_text_typography.h>
#include <native_drawing/drawing_font_collection.h>

namespace rnoh {

using namespace facebook;

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

using UniqueTypographyStyle = std::unique_ptr<OH_Drawing_TypographyStyle, decltype(&OH_Drawing_DestroyTypographyStyle)>;

class NativeTypography final {
  public:
    facebook::react::TextMeasurement::Attachments getAttachments() const {
        facebook::react::TextMeasurement::Attachments result;
        result.reserve(m_attachmentCount);
        auto placeholderRects = OH_Drawing_TypographyGetRectsForPlaceholders(m_typography.get());

        // calculate attachment sizes and positions
        for (auto i = 0; i < m_attachmentCount; i++) {
            react::TextMeasurement::Attachment attachment;
            attachment.frame.origin.x = OH_Drawing_GetLeftFromTextBox(placeholderRects, i);
            attachment.frame.origin.y = OH_Drawing_GetTopFromTextBox(placeholderRects, i);
            attachment.frame.size.width =
                OH_Drawing_GetRightFromTextBox(placeholderRects, i) - OH_Drawing_GetLeftFromTextBox(placeholderRects, i);
            attachment.frame.size.height =
                OH_Drawing_GetBottomFromTextBox(placeholderRects, i) - OH_Drawing_GetTopFromTextBox(placeholderRects, i);
            result.push_back(std::move(attachment));
        }
        return result;
    }

    facebook::react::Float getHeight() const {
        return OH_Drawing_TypographyGetHeight(m_typography.get());
    }

    facebook::react::Float getLongestLineWidth() const {
        return OH_Drawing_TypographyGetLongestLine(m_typography.get());
    }

  private:
    NativeTypography(
        OH_Drawing_TypographyCreate *typographyHandler,
        size_t attachmentCount,
        facebook::react::Float maxWidth) : m_typography(OH_Drawing_CreateTypography(typographyHandler), OH_Drawing_DestroyTypography),
                                                          m_attachmentCount(attachmentCount) {
        OH_Drawing_TypographyLayout(m_typography.get(), maxWidth);
    }

    std::unique_ptr<OH_Drawing_Typography, decltype(&OH_Drawing_DestroyTypography)> m_typography;
    size_t m_attachmentCount;

    friend class NativeTypographyBuilder;
};

class NativeTypographyBuilder final {
  public:
    NativeTypographyBuilder(OH_Drawing_TypographyStyle *typographyStyle) : m_typographyHandler(OH_Drawing_CreateTypographyHandler(typographyStyle, OH_Drawing_CreateFontCollection()), OH_Drawing_DestroyTypographyHandler) {}

    void setMaximumWidth(facebook::react::Float maximumWidth) {
        if (!isnan(maximumWidth) && maximumWidth > 0) {
            m_maximumWidth = maximumWidth;
        } else {
            m_maximumWidth = std::numeric_limits<facebook::react::Float>::max();
        }
    }

    void addFragment(const react::AttributedString::Fragment &fragment) {
        if (!fragment.isAttachment()) {
            addTextFragment(fragment);
        } else {
            addAttachment(fragment);
        }
    }

    NativeTypography build() const {
        return NativeTypography(m_typographyHandler.get(), m_attachmentCount, m_maximumWidth);
    }

  private:
    void addTextFragment(const react::AttributedString::Fragment &fragment) {
        auto textStyle = OH_Drawing_CreateTextStyle();
        auto fontSize = fragment.textAttributes.fontSize;
        if (fontSize <= 0) {
            // set fontSize to default for negative values(same as iOS)
            fontSize = 14;
        }
        OH_Drawing_SetTextStyleFontSize(textStyle, fontSize);
        // new NDK for setting letterSpacing
        if (!isnan(fragment.textAttributes.letterSpacing)) {
            OH_Drawing_SetTextStyleLetterSpacing(textStyle, fragment.textAttributes.letterSpacing);
        }
        if (!isnan(fragment.textAttributes.lineHeight) && fragment.textAttributes.lineHeight > 0) {
            // fontSize * fontHeight = lineHeight, no direct ndk for setting lineHeight so do it
            // in this weird way
            double fontHeight = fragment.textAttributes.lineHeight / fontSize;
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
        OH_Drawing_TypographyHandlerPushTextStyle(m_typographyHandler.get(), textStyle);
        OH_Drawing_TypographyHandlerAddText(m_typographyHandler.get(), fragment.string.c_str());
        OH_Drawing_DestroyTextStyle(textStyle);
    }

    void addAttachment(const react::AttributedString::Fragment &fragment) {
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
        OH_Drawing_TypographyHandlerAddPlaceholder(m_typographyHandler.get(), &inlineView);
        m_attachmentCount++;
    }

    std::unique_ptr<OH_Drawing_TypographyCreate, decltype(&OH_Drawing_DestroyTypographyHandler)> m_typographyHandler;
    size_t m_attachmentCount = 0;
    facebook::react::Float m_maximumWidth = std::numeric_limits<facebook::react::Float>::max();
};

react::TextMeasurement TextMeasurer::measure(react::AttributedString attributedString,
                                             react::ParagraphAttributes paragraphAttributes,
                                             react::LayoutConstraints layoutConstraints) {
    auto const &fragments = attributedString.getFragments();
    auto canUseOHOSTextMeasurer = fragments.size() == 1 && !fragments[0].isAttachment() &&
                                  isnan(fragments[0].textAttributes.letterSpacing) &&
                                  isnan(fragments[0].textAttributes.lineHeight);
    auto isNDKTextMeasuringEnabled = this->m_featureFlagRegistry->getFeatureFlagStatus("ENABLE_NDK_TEXT_MEASURING");
    if (canUseOHOSTextMeasurer || isNDKTextMeasuringEnabled) {
        UniqueTypographyStyle typographyStyle(OH_Drawing_CreateTypographyStyle(), OH_Drawing_DestroyTypographyStyle);
        if (paragraphAttributes.maximumNumberOfLines) {
            OH_Drawing_SetTypographyTextMaxLines(typographyStyle.get(), paragraphAttributes.maximumNumberOfLines);
        }
        NativeTypographyBuilder typographyBuilder(typographyStyle.get());
        for (auto const &fragment : attributedString.getFragments()) {
            typographyBuilder.addFragment(fragment);
        }
        typographyBuilder.setMaximumWidth(layoutConstraints.maximumSize.width);
        auto typography = typographyBuilder.build();
        auto height = typography.getHeight();
        auto longestLineWidth = typography.getLongestLineWidth();
        return {{.width = longestLineWidth + 0.5, .height = height}, typography.getAttachments()};
    } else {
        react::TextMeasurement result = {{0, 0}, {}};
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
        return result;
    }
}

} // namespace rnoh
