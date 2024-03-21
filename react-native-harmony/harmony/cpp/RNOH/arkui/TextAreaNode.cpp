#include "TextAreaNode.h"
#include "NativeNodeApi.h"

namespace rnoh {

TextAreaNode::TextAreaNode()
    : TextInputNodeBase(ArkUI_NodeType::ARKUI_NODE_TEXT_AREA),
      m_childArkUINodeHandle(nullptr) {
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_TEXT_AREA_ON_PASTE, 0));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_TEXT_AREA_ON_CHANGE, 1));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_FOCUS, 2));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_BLUR, 3));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_TEXT_AREA_ON_TEXT_SELECTION_CHANGE, 4));
}

TextAreaNode::~TextAreaNode() {
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_TEXT_AREA_ON_PASTE);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_TEXT_AREA_ON_CHANGE);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_FOCUS);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_BLUR);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_TEXT_AREA_ON_TEXT_SELECTION_CHANGE);
}

void TextAreaNode::onNodeEvent(ArkUI_NodeEvent *event) {
    if (event->kind == ArkUI_NodeEventType::NODE_TEXT_AREA_ON_PASTE) {
        if (m_textAreaNodeDelegate != nullptr) {
            m_textAreaNodeDelegate->onPaste();
        }
    } else if (event->kind == ArkUI_NodeEventType::NODE_TEXT_AREA_ON_CHANGE) {
        if (m_textAreaNodeDelegate != nullptr) {
            m_textAreaNodeDelegate->onChange();
        }
    } else if (event->kind == ArkUI_NodeEventType::NODE_ON_FOCUS) {
        if (m_textAreaNodeDelegate != nullptr) {
            m_textAreaNodeDelegate->onFocus();
        }
    } else if (event->kind == ArkUI_NodeEventType::NODE_ON_BLUR) {
        if (m_textAreaNodeDelegate != nullptr) {
            m_textAreaNodeDelegate->onBlur();
        }
    } else if (event->kind == ArkUI_NodeEventType::NODE_TEXT_AREA_ON_TEXT_SELECTION_CHANGE) {
        if (m_textAreaNodeDelegate != nullptr) {
            m_textAreaNodeDelegate->onTextSelectionChange();
        }
    }
}

void TextAreaNode::setTextAreaNodeDelegate(TextAreaNodeDelegate *textAreaNodeDelegate) {
    m_textAreaNodeDelegate = textAreaNodeDelegate;
}

facebook::react::Point TextAreaNode::getTextAreaOffset() const {
    auto item = NativeNodeApi::getInstance()->getAttribute(m_nodeHandle, NODE_OFFSET);
    facebook::react::Float x = item->value[0].i32;
    facebook::react::Float y = item->value[1].i32;
    return facebook::react::Point{x, y};
}

void TextAreaNode::setTextContent(std::string const &textContent) {
    ArkUI_AttributeItem item = { .string = textContent.c_str() };
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_AREA_TEXT, &item));
}

void TextAreaNode::setInputType(facebook::react::KeyboardType keyboardType) {
    if (keyboardType_ == keyboardType) {
        return;
    }
    ArkUI_NumberValue value[] = {{.i32 = static_cast<int32_t>(convertTextAreaInputType(keyboardType))}};
    ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_AREA_TYPE, &item));
    keyboardType_ = keyboardType;
}

ArkUI_TextAreaType TextAreaNode::convertTextAreaInputType(facebook::react::KeyboardType keyboardType) {
    switch (keyboardType) {
    case facebook::react::KeyboardType::Numeric:
    case facebook::react::KeyboardType::NumberPad:
        return ARKUI_TEXTAREA_TYPE_NUMBER;
    case facebook::react::KeyboardType::PhonePad:
        return ARKUI_TEXTAREA_TYPE_PHONE_NUMBER;
    case facebook::react::KeyboardType::EmailAddress:
        return ARKUI_TEXTAREA_TYPE_EMAIL;
    default:
        return ARKUI_TEXTAREA_TYPE_NORMAL;
    }
}
} // namespace rnoh
