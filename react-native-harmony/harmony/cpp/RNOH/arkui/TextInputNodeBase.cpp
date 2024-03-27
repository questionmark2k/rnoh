#include "TextInputNodeBase.h"
#include "NativeNodeApi.h"

namespace rnoh {

TextInputNodeBase::TextInputNodeBase(ArkUI_NodeType nodeType)
    : ArkUINode(NativeNodeApi::getInstance()->createNode(nodeType)) {}

void TextInputNodeBase::setPadding(int32_t padding) {
    ArkUI_NumberValue value[] = { (float)padding };
    ArkUI_AttributeItem item = { value, sizeof(value)/sizeof(ArkUI_NumberValue) };
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_PADDING, &item));
}

void TextInputNodeBase::setFocusable(bool const &focusable) {
    int32_t focusableValue = 1;
    if (!focusable) {
        focusableValue = 0;
    }
    ArkUI_NumberValue value[] = {{ .i32 = focusableValue }};
    ArkUI_AttributeItem item = { value, sizeof(value) / sizeof(ArkUI_NumberValue) };
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_FOCUSABLE, &item));
}

void TextInputNodeBase::setAutoFocus(bool const &autoFocus) {
    ArkUI_NumberValue value[] = { { .i32 = autoFocus } };
    ArkUI_AttributeItem item = {value, sizeof(value)/sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_FOCUS_STATUS, &item));
}

void TextInputNodeBase::setResponseRegion(facebook::react::Point const &position,
        facebook::react::Size const &size) {
    ArkUI_NumberValue value[] = {0.0f, 0.0f, (float)size.width, (float)size.height};
    ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_RESPONSE_REGION, &item));
}

void TextInputNodeBase::setFocusStatus(int const &focus) {
    ArkUI_NumberValue value[] = { { .i32 = focus } };
    ArkUI_AttributeItem item = {value, sizeof(value)/sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_FOCUS_STATUS, &item));
}

void TextInputNodeBase::setFontColor(facebook::react::SharedColor const &color) {
    uint32_t colorValue = *color;
    ArkUI_NumberValue preparedColorValue[] = {{.u32 = colorValue}};
    ArkUI_AttributeItem colorItem = {preparedColorValue, sizeof(preparedColorValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_FONT_COLOR, &colorItem));
}

void TextInputNodeBase::setFont(facebook::react::TextAttributes const &textAttributes, bool multiline) {
    std::string fontFamily = "HarmonyOS Sans";
    if (textAttributes.fontFamily != "") {
        fontFamily = textAttributes.fontFamily;
    }
    ArkUI_AttributeItem itemFamily = {.string = fontFamily.c_str()};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_FONT_FAMILY, &itemFamily));

    ArkUI_NumberValue valueSize[] = { (float)textAttributes.fontSize };
    ArkUI_AttributeItem itemSize = { valueSize, sizeof(valueSize) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_FONT_SIZE, &itemSize));

    ArkUI_NumberValue fontWeight = {.i32 = ARKUI_FONT_WEIGHT_NORMAL };
    if (textAttributes.fontWeight) {
        fontWeight = convertFontWeight((int)textAttributes.fontWeight.value());
    }
    ArkUI_NumberValue valueWeight[] = {fontWeight};
    ArkUI_AttributeItem itemWeight = { valueWeight, 1 };
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_FONT_WEIGHT, &itemWeight));

    ArkUI_NumberValue tempStyle = {.i32 = ARKUI_FONT_STYLE_NORMAL};
    if (textAttributes.fontStyle == facebook::react::FontStyle::Italic) {
        tempStyle = {.i32 = ARKUI_FONT_STYLE_ITALIC};
    }
    ArkUI_NumberValue valueStyle[] = {tempStyle};
    ArkUI_AttributeItem itemStyle = { valueStyle, 1 };
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_FONT_STYLE, &itemStyle));

    ArkUI_NumberValue value[] = { (float)textAttributes.fontSize, tempStyle, fontWeight };
    ArkUI_AttributeItem item = { value, sizeof(value)/sizeof(ArkUI_NumberValue), fontFamily.c_str() };
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, 
        multiline ? NODE_TEXT_AREA_PLACEHOLDER_FONT : NODE_TEXT_INPUT_PLACEHOLDER_FONT, &item));
}

void TextInputNodeBase::setCaretColor(facebook::react::SharedColor const &color, bool multiline) {
    uint32_t colorValue = *color;
    ArkUI_NumberValue preparedColorValue[] = {{.u32 = colorValue}};
    ArkUI_AttributeItem colorItem = {preparedColorValue, sizeof(preparedColorValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, 
        multiline ? NODE_TEXT_AREA_CARET_COLOR : NODE_TEXT_INPUT_CARET_COLOR, &colorItem));
}

void TextInputNodeBase::setEnabled(bool const &enabled) {
    ArkUI_NumberValue value[] = { { .i32 = enabled } };
    ArkUI_AttributeItem item = {value, sizeof(value)/sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_ENABLED, &item));
}

void TextInputNodeBase::setMaxLength(int const &maxLength, bool multiline) {
    ArkUI_NumberValue value[] = { { .i32 = maxLength } };
    ArkUI_AttributeItem item = {value, sizeof(value)/sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, 
        multiline ? NODE_TEXT_AREA_MAX_LENGTH : NODE_TEXT_INPUT_MAX_LENGTH, &item));
}

void TextInputNodeBase::setPlaceHolder(std::string const &placeHodler, bool multiline) {
    ArkUI_AttributeItem item = { .string = placeHodler.c_str() };
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, 
        multiline ? NODE_TEXT_AREA_PLACEHOLDER : NODE_TEXT_INPUT_PLACEHOLDER, &item));
}

void TextInputNodeBase::setPlaceHolderColor(facebook::react::SharedColor const &color, bool multiline) {
    uint32_t colorValue = *color;
    ArkUI_NumberValue preparedColorValue[] = {{.u32 = colorValue}};
    ArkUI_AttributeItem colorItem = {preparedColorValue, sizeof(preparedColorValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, 
        multiline ? NODE_TEXT_AREA_PLACEHOLDER_COLOR : NODE_TEXT_INPUT_PLACEHOLDER_COLOR, &colorItem));
}

void TextInputNodeBase::setTextAlign(std::optional<facebook::react::TextAlignment> const &textAlign) {
    ArkUI_NumberValue alignMent[] = {convertTextAlign(textAlign.value())};
    ArkUI_AttributeItem item = { .value = alignMent, .size = 1};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_ALIGN, &item));
}

ArkUI_NumberValue TextInputNodeBase::convertTextAlign(facebook::react::TextAlignment alignment) {
    switch (alignment) {
        case facebook::react::TextAlignment::Natural:
        case facebook::react::TextAlignment::Left:
            return {.i32 = static_cast<int32_t>(ARKUI_TEXT_ALIGNMENT_START)};
        case facebook::react::TextAlignment::Right:
            return {.i32 = static_cast<int32_t>(ARKUI_TEXT_ALIGNMENT_END)};
        case facebook::react::TextAlignment::Center:
            return {.i32 = static_cast<int32_t>(ARKUI_TEXT_ALIGNMENT_CENTER)};
        case facebook::react::TextAlignment::Justified:
            return {.i32 = static_cast<int32_t>(ARKUI_TEXT_ALIGNMENT_JUSTIFY)};
        default:
            return {.i32 = static_cast<int32_t>(ARKUI_TEXT_ALIGNMENT_START)};
    }
}

ArkUI_NumberValue TextInputNodeBase::convertFontWeight(int fontWeight) {
    if (fontWeight < 200) {
        return {.i32 = ARKUI_FONT_WEIGHT_W100};
    }
    if (fontWeight < 300) {
        return {.i32 = ARKUI_FONT_WEIGHT_W200};
    }
    if (fontWeight < 400) {
        return {.i32 = ARKUI_FONT_WEIGHT_W300};
    }
    if (fontWeight < 500) {
        return {.i32 = ARKUI_FONT_WEIGHT_W400};
    }
    if (fontWeight < 600) {
        return {.i32 = ARKUI_FONT_WEIGHT_W500};
    }
    if (fontWeight < 700) {
        return {.i32 = ARKUI_FONT_WEIGHT_W600};
    }
    if (fontWeight < 800) {
        return {.i32 = ARKUI_FONT_WEIGHT_W700};
    }
    if (fontWeight < 900) {
        return {.i32 = ARKUI_FONT_WEIGHT_W800};
    }
    return {.i32 = ARKUI_FONT_WEIGHT_W900};
}
} // namespace rnoh
