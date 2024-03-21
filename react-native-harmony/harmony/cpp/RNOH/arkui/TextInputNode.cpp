#include "TextInputNode.h"
#include "NativeNodeApi.h"

namespace rnoh {

TextInputNode::TextInputNode()
    : TextInputNodeBase(ArkUI_NodeType::ARKUI_NODE_TEXT_INPUT),
      m_childArkUINodeHandle(nullptr) {
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_TEXT_INPUT_ON_PASTE, 0));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_TEXT_INPUT_ON_CHANGE, 1));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_TEXT_INPUT_ON_SUBMIT, 2));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_FOCUS, 3));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_BLUR, 4));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_TEXT_INPUT_ON_TEXT_SELECTION_CHANGE, 5));
}

TextInputNode::~TextInputNode() {
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_TEXT_INPUT_ON_PASTE);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_TEXT_INPUT_ON_CHANGE);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_TEXT_INPUT_ON_SUBMIT);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_FOCUS);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_BLUR);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_TEXT_INPUT_ON_TEXT_SELECTION_CHANGE);
}

void TextInputNode::onNodeEvent(ArkUI_NodeEvent *event) {
    if (event->kind == ArkUI_NodeEventType::NODE_TEXT_INPUT_ON_PASTE) {
        if (m_textInputNodeDelegate != nullptr) {
            m_textInputNodeDelegate->onPaste();
        }
    } else if (event->kind == ArkUI_NodeEventType::NODE_TEXT_INPUT_ON_CHANGE) {
        if (m_textInputNodeDelegate != nullptr) {
            m_textInputNodeDelegate->onChange();
        }
    } else if (event->kind == ArkUI_NodeEventType::NODE_TEXT_INPUT_ON_SUBMIT) {
        if (m_textInputNodeDelegate != nullptr) {
            m_textInputNodeDelegate->onSubmit();
        }
    } else if (event->kind == ArkUI_NodeEventType::NODE_ON_FOCUS) {
        if (m_textInputNodeDelegate != nullptr) {
            m_textInputNodeDelegate->onFocus();
        }
    } else if (event->kind == ArkUI_NodeEventType::NODE_ON_BLUR) {
        if (m_textInputNodeDelegate != nullptr) {
            m_textInputNodeDelegate->onBlur();
        }
    } else if (event->kind == ArkUI_NodeEventType::NODE_TEXT_INPUT_ON_TEXT_SELECTION_CHANGE) {
        if (m_textInputNodeDelegate != nullptr) {
            m_textInputNodeDelegate->onTextSelectionChange();
        }
    }
}

void TextInputNode::setTextInputNodeDelegate(TextInputNodeDelegate *textInputNodeDelegate) {
    m_textInputNodeDelegate = textInputNodeDelegate;
}

facebook::react::Point TextInputNode::getTextInputOffset() const {
    auto item = NativeNodeApi::getInstance()->getAttribute(m_nodeHandle, NODE_OFFSET);
    facebook::react::Float x = item->value[0].i32;
    facebook::react::Float y = item->value[1].i32;
    return facebook::react::Point{x, y};
}

void TextInputNode::setTextContent(std::string const &textContent) {
    ArkUI_AttributeItem item = { .string = textContent.c_str() };
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_INPUT_TEXT, &item));
}

void TextInputNode::setTextSelection(int const &start, int const &end) {
    ArkUI_NumberValue value[] = { { .i32 = start }, { .i32 = end } };
    ArkUI_AttributeItem item = {value, sizeof(value)/sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_INPUT_TEXT_SELECTION, &item));
}

void TextInputNode::setSelectedBackgroundColor(facebook::react::SharedColor const &color) {
    ArkUI_NumberValue selectedBackgroundColor[] = {{.u32 = convertColorToTranslucent(color)}};
    ArkUI_AttributeItem colorItem = {selectedBackgroundColor, sizeof(selectedBackgroundColor) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_INPUT_SELECTED_BACKGROUND_COLOR, &colorItem));
}

void TextInputNode::setCaretHidden(bool hidden) {
    if (hidden) {
        ArkUI_NumberValue value[] = { { .f32 = 0 } };
        ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_INPUT_CARET_STYLE, &item));
    } else {
        maybeThrow(NativeNodeApi::getInstance()->resetAttribute(m_nodeHandle, NODE_TEXT_INPUT_CARET_STYLE));
    }
}

void TextInputNode::setInputType(bool secure, facebook::react::KeyboardType keyboardType) {
    if ((m_secureInput == secure) && (m_keyboardType == keyboardType)) {
        return;
    }
    ArkUI_NumberValue defaultValue[] = {{.i32 = static_cast<int32_t>(ARKUI_TEXTINPUT_TYPE_NORMAL)}};
    ArkUI_AttributeItem item = {defaultValue};
    if (secure) {
        if (m_secureInput != secure) {
            ArkUI_NumberValue value[] = {{.i32 = static_cast<int32_t>(ARKUI_TEXTINPUT_TYPE_PASSWORD)}};
            item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
            maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_INPUT_TYPE, &item));
            m_secureInput = secure;
        }
        return;
    }
    if (keyboardType != m_keyboardType) {
        ArkUI_NumberValue value[] = {{.i32 = static_cast<int32_t>(convertInputType(keyboardType))}};
        item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_INPUT_TYPE, &item));
        m_keyboardType = keyboardType;
    }
}

void TextInputNode::setPasswordIconVisibility(bool isVisible) {
    ArkUI_NumberValue value[] = {{.i32 = isVisible ? 1 : 0}};
    ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_INPUT_SHOW_PASSWORD_ICON, &item));
}

void TextInputNode::setEnterKeyType(facebook::react::ReturnKeyType returnKeyType) {
    ArkUI_NumberValue value[] = {{.i32 = static_cast<int32_t>(convertEnterKeyType(returnKeyType))}};
    ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_INPUT_ENTER_KEY_TYPE, &item));
}

void TextInputNode::setCancelButtonMode(facebook::react::TextInputAccessoryVisibilityMode mode) {
    ArkUI_AttributeItem item;
    if (mode == facebook::react::TextInputAccessoryVisibilityMode::Always) {
        ArkUI_NumberValue value[] = {{.i32 = static_cast<int32_t>(ARKUI_CANCELBUTTON_STYLE_INPUT)}};
        item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
    } else {
        ArkUI_NumberValue value[] = {{.i32 = static_cast<int32_t>(ARKUI_CANCELBUTTON_STYLE_INVISIBLE)}};
        item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
    }
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_INPUT_CANCEL_BUTTON, &item));
}

uint32_t TextInputNode::convertColorToTranslucent(facebook::react::SharedColor const &color) {
    auto colorComponents = colorComponentsFromColor(color);
    uint32_t red = static_cast<uint32_t>(colorComponents.red * 255) & 0xFF;
    uint32_t green = static_cast<uint32_t>(colorComponents.green * 255) & 0xFF;
    uint32_t blue = static_cast<uint32_t>(colorComponents.blue * 255) & 0xFF;
    uint32_t alpha = 0x55;
    auto colorValue = (alpha << 24) | (red << 16) | (green << 8) | blue;
    return colorValue;
}

ArkUI_TextInputType TextInputNode::convertInputType(facebook::react::KeyboardType keyboardType) {
    switch (keyboardType) {
    case facebook::react::KeyboardType::Numeric:
    case facebook::react::KeyboardType::NumberPad:
        return ARKUI_TEXTINPUT_TYPE_NUMBER;
    case facebook::react::KeyboardType::DecimalPad:
        return ARKUI_TEXTINPUT_TYPE_NUMBER_DECIMAL;
    case facebook::react::KeyboardType::PhonePad:
        return ARKUI_TEXTINPUT_TYPE_PHONE_NUMBER;
    case facebook::react::KeyboardType::EmailAddress:
        return ARKUI_TEXTINPUT_TYPE_EMAIL;
    default:
        return ARKUI_TEXTINPUT_TYPE_NORMAL;
    }
}

ArkUI_EnterKeyType TextInputNode::convertEnterKeyType(facebook::react::ReturnKeyType returnKeyType) {
    switch (returnKeyType) {
    case facebook::react::ReturnKeyType::Done:
        return ARKUI_ENTER_KEY_TYPE_DONE;
    case facebook::react::ReturnKeyType::Go:
        return ARKUI_ENTER_KEY_TYPE_GO;
    case facebook::react::ReturnKeyType::Next:
        return ARKUI_ENTER_KEY_TYPE_NEXT;
    case facebook::react::ReturnKeyType::Search:
        return ARKUI_ENTER_KEY_TYPE_SEARCH;
    case facebook::react::ReturnKeyType::Send:
        return ARKUI_ENTER_KEY_TYPE_SEND;
    default:
        return ARKUI_ENTER_KEY_TYPE_DONE;
    }
}
} // namespace rnoh
