#include "TextAreaNode.h"
#include "NativeNodeApi.h"
#include "RNOH/arkui/conversions.h"

static constexpr ArkUI_NodeEventType TEXT_AREA_NODE_EVENT_TYPES[] = {
    NODE_TEXT_AREA_ON_PASTE,
    NODE_TEXT_AREA_ON_CHANGE,
    NODE_ON_FOCUS,
    NODE_ON_BLUR,
    NODE_TEXT_AREA_ON_TEXT_SELECTION_CHANGE};

namespace rnoh {

TextAreaNode::TextAreaNode()
    : TextInputNodeBase(ArkUI_NodeType::ARKUI_NODE_TEXT_AREA),
      m_childArkUINodeHandle(nullptr) {
  for (auto eventType : TEXT_AREA_NODE_EVENT_TYPES) {
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(
        m_nodeHandle, eventType, eventType));
  }
}

TextAreaNode::~TextAreaNode() {
  for (auto eventType : TEXT_AREA_NODE_EVENT_TYPES) {
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, eventType);
  }
}

void TextAreaNode::onNodeEvent(ArkUI_NodeEvent* event) {
  if (event->kind == ArkUI_NodeEventType::NODE_TEXT_AREA_ON_PASTE) {
    if (m_textAreaNodeDelegate != nullptr) {
      m_textAreaNodeDelegate->onPaste();
    }
  } else if (event->kind == ArkUI_NodeEventType::NODE_TEXT_AREA_ON_CHANGE) {
    if (m_textAreaNodeDelegate != nullptr) {
      m_textAreaNodeDelegate->onChange(event);
    }
  } else if (event->kind == ArkUI_NodeEventType::NODE_ON_FOCUS) {
    if (m_textAreaNodeDelegate != nullptr) {
      m_textAreaNodeDelegate->onFocus(event);
    }
  } else if (event->kind == ArkUI_NodeEventType::NODE_ON_BLUR) {
    if (m_textAreaNodeDelegate != nullptr) {
      m_textAreaNodeDelegate->onBlur(event);
    }
  } else if (
      event->kind ==
      ArkUI_NodeEventType::NODE_TEXT_AREA_ON_TEXT_SELECTION_CHANGE) {
    if (m_textAreaNodeDelegate != nullptr) {
      m_textAreaNodeDelegate->onTextSelectionChange();
    }
  }
}

void TextAreaNode::setTextAreaNodeDelegate(
    TextAreaNodeDelegate* textAreaNodeDelegate) {
  m_textAreaNodeDelegate = textAreaNodeDelegate;
}

facebook::react::Point TextAreaNode::getTextAreaOffset() const {
  auto item =
      NativeNodeApi::getInstance()->getAttribute(m_nodeHandle, NODE_OFFSET);
  facebook::react::Float x = item->value[0].i32;
  facebook::react::Float y = item->value[1].i32;
  return facebook::react::Point{x, y};
}

void TextAreaNode::setTextContent(std::string const& textContent) {
  ArkUI_AttributeItem item = {.string = textContent.c_str()};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_AREA_TEXT, &item));
}

void TextAreaNode::setInputType(facebook::react::KeyboardType keyboardType) {
  ArkUI_NumberValue value[] = {
      {.i32 =
           static_cast<int32_t>(rnoh::convertTextAreaInputType(keyboardType))}};
  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_AREA_TYPE, &item));
}

void TextAreaNode::setFont(
    facebook::react::TextAttributes const& textAttributes) {
  TextInputNodeBase::setCommonFontAttributes(textAttributes);

  std::string fontFamily = "HarmonyOS Sans";
  if (!textAttributes.fontFamily.empty()) {
    fontFamily = textAttributes.fontFamily;
  }

  ArkUI_NumberValue fontWeight = {.i32 = ARKUI_FONT_WEIGHT_NORMAL};
  if (textAttributes.fontWeight) {
    fontWeight =
        rnoh::convertFontWeight((int32_t)textAttributes.fontWeight.value());
  }

  ArkUI_NumberValue tempStyle = {.i32 = ARKUI_FONT_STYLE_NORMAL};
  if (textAttributes.fontStyle == facebook::react::FontStyle::Italic) {
    tempStyle = {.i32 = ARKUI_FONT_STYLE_ITALIC};
  }

  ArkUI_NumberValue value[] = {
      (float)textAttributes.fontSize, tempStyle, fontWeight};
  ArkUI_AttributeItem item = {
      value, sizeof(value) / sizeof(ArkUI_NumberValue), fontFamily.c_str()};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_AREA_PLACEHOLDER_FONT, &item));
}

void TextAreaNode::setCaretColor(facebook::react::SharedColor const& color) {
  uint32_t colorValue = *color;
  ArkUI_NumberValue preparedColorValue[] = {{.u32 = colorValue}};
  ArkUI_AttributeItem colorItem = {
      preparedColorValue,
      sizeof(preparedColorValue) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_AREA_CARET_COLOR, &colorItem));
}

void TextAreaNode::setMaxLength(int32_t maxLength) {
  ArkUI_NumberValue value[] = {{.i32 = maxLength}};
  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_AREA_MAX_LENGTH, &item));
}

void TextAreaNode::setPlaceholder(std::string const& placeholder) {
  ArkUI_AttributeItem item = {.string = placeholder.c_str()};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_AREA_PLACEHOLDER, &item));
}

void TextAreaNode::setPlaceholderColor(
    facebook::react::SharedColor const& color) {
  uint32_t colorValue = *color;
  ArkUI_NumberValue preparedColorValue[] = {{.u32 = colorValue}};
  ArkUI_AttributeItem colorItem = {
      preparedColorValue,
      sizeof(preparedColorValue) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_TEXT_AREA_PLACEHOLDER_COLOR, &colorItem));
}

void TextAreaNode::defaultSetPadding() {
  ArkUI_NumberValue value[] = {0};
  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_PADDING, &item));
}

std::string TextAreaNode::getTextContent() {
  auto item = NativeNodeApi::getInstance()->getAttribute(
      m_nodeHandle, NODE_TEXT_AREA_TEXT);
  return item->string;
}
} // namespace rnoh
