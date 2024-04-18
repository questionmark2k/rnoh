#include "SpanNode.h"
#include "NativeNodeApi.h"

namespace rnoh {
ImageSpanNode::ImageSpanNode()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(
          ArkUI_NodeType::ARKUI_NODE_IMAGE_SPAN)) {}

SpanNode::SpanNode()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(
          ArkUI_NodeType::ARKUI_NODE_SPAN)) {}

SpanNode& SpanNode::setSpanContent(const std::string& text) {
  ArkUI_AttributeItem item = {.string = text.c_str()};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_SPAN_CONTENT, &item));
  return *this;
}

SpanNode& SpanNode::setFontColor(uint32_t color) {
  ArkUI_NumberValue value[] = {{.u32 = color}};
  ArkUI_AttributeItem item = {.value = value, .size = 1};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_FONT_COLOR, &item));
  return *this;
}

SpanNode& SpanNode::setFontSize(float fontSize) {
  ArkUI_NumberValue value[] = {{.f32 = fontSize}};
  ArkUI_AttributeItem item = {.value = value, .size = 1};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_FONT_SIZE, &item));
  return *this;
}

SpanNode& SpanNode::setFontStyle(int32_t fontStyle) {
  ArkUI_NumberValue value[] = {{.i32 = fontStyle}};
  ArkUI_AttributeItem item = {.value = value, .size = 1};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_FONT_STYLE, &item));
  return *this;
}

SpanNode& SpanNode::setFontWeight(int32_t fontWeight) {
  ArkUI_NumberValue value[] = {{.i32 = fontWeight}};
  ArkUI_AttributeItem item = {.value = value, .size = 1};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_FONT_WEIGHT, &item));
  return *this;
}

SpanNode& SpanNode::setTextLineHeight(float textLineHeight) {
  ArkUI_NumberValue value[] = {{.f32 = textLineHeight}};
  ArkUI_AttributeItem item = {.value = value, .size = 1};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_LINE_HEIGHT, &item));
  return *this;
}

SpanNode& SpanNode::setTextDecoration(
    int32_t decorationStyle,
    uint32_t decorationColor /*= 0xFFFF0000*/) {
  ArkUI_NumberValue value[] = {
      {.i32 = decorationStyle}, {.u32 = decorationColor}};
  ArkUI_AttributeItem item = {
      .value = value, .size = sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_DECORATION, &item));
  return *this;
}

SpanNode& SpanNode::setTextCase(int32_t textCase) {
  ArkUI_NumberValue value[] = {{.i32 = textCase}};
  ArkUI_AttributeItem item = {.value = value, .size = 1};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_CASE, &item));
  return *this;
}

SpanNode& SpanNode::setTextLetterSpacing(float textLetterSpacing) {
  ArkUI_NumberValue value[] = {{.f32 = textLetterSpacing}};
  ArkUI_AttributeItem item = {.value = value, .size = 1};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_LETTER_SPACING, &item));
  return *this;
}

SpanNode& SpanNode::setFontFamily(const std::string& fontFamily) {
  ArkUI_AttributeItem item = {.string = fontFamily.c_str()};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_FONT_FAMILY, &item));
  return *this;
}

SpanNode& SpanNode::setTextShadow(
    float textShadowRadius,
    int32_t textShadowType,
    uint32_t textShadowColor,
    float textShadowOffsetX,
    float textShadowOffsetY) {
  ArkUI_NumberValue value[] = {
      {.f32 = textShadowRadius},
      {.i32 = textShadowType},
      {.u32 = textShadowColor},
      {.f32 = textShadowOffsetX},
      {.f32 = textShadowOffsetY}};
  ArkUI_AttributeItem item = {
      .value = value, .size = sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_TEXT_SHADOW, &item));
  return *this;
}

SpanNode& SpanNode::setBackgroundStyle(uint32_t color) {
  ArkUI_NumberValue value[] = {{.u32 = color}, {.f32 = 0}};
  ArkUI_AttributeItem item = {
      .value = value, .size = sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_SPAN_TEXT_BACKGROUND_STYLE, &item));
  return *this;
}

} // namespace rnoh
