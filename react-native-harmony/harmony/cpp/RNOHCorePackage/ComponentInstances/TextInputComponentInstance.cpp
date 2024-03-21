#include "TextInputComponentInstance.h"

#include <sstream>
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/textinput/TextInputProps.h>
#include <react/renderer/components/textinput/TextInputState.h>
#include <glog/logging.h>
#include <folly/dynamic.h>

namespace rnoh {

TextInputComponentInstance::TextInputComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {
    m_textInputNode.setTextInputNodeDelegate(this);
    m_textAreaNode.setTextAreaNodeDelegate(this);
}

void TextInputComponentInstance::onChange() {
    m_eventEmitter->onChange(getTextInputMetrics());
}

void TextInputComponentInstance::onSubmit() {
    m_eventEmitter->onSubmitEditing(getTextInputMetrics());
}

void TextInputComponentInstance::onBlur() {
    m_eventEmitter->onBlur(getTextInputMetrics());
}

void TextInputComponentInstance::onFocus() {
    m_eventEmitter->onFocus(getTextInputMetrics());
}

facebook::react::TextInputMetrics TextInputComponentInstance::getTextInputMetrics() {
    auto textInputMetrics = facebook::react::TextInputMetrics();
    textInputMetrics.contentOffset = multiline_ ?
        m_textAreaNode.getTextAreaOffset() : m_textInputNode.getTextInputOffset();
    textInputMetrics.containerSize = m_layoutMetrics.frame.size;
    textInputMetrics.eventCount = 1;
    textInputMetrics.zoomScale = 1;

    return textInputMetrics;
}

void TextInputComponentInstance::onPropsChanged(std::shared_ptr<const facebook::react::TextInputProps> const &props) {
    CppComponentInstance::onPropsChanged(props);
    auto p = std::dynamic_pointer_cast<const facebook::react::TextInputProps>(props);
    multiline_ = props->traits.multiline;
    if (!m_props || *(props->textAttributes.foregroundColor) != *(m_props->textAttributes.foregroundColor)) {
        if (props->textAttributes.foregroundColor) {
            m_textAreaNode.setFontColor(props->textAttributes.foregroundColor);
            m_textInputNode.setFontColor(props->textAttributes.foregroundColor);
        } else {
            m_textAreaNode.setFontColor(facebook::react::blackColor());
            m_textInputNode.setFontColor(facebook::react::blackColor());
        }
    }
    if (!m_props || props->textAttributes != m_props->textAttributes) {
        m_textAreaNode.setFont(props->textAttributes, multiline_);
        m_textInputNode.setFont(props->textAttributes, multiline_);
    }
    if (!m_props || *(props->backgroundColor) != *(m_props->backgroundColor)) {
        if (props->backgroundColor) {
            m_textAreaNode.setBackgroundColor(props->backgroundColor);
            m_textInputNode.setBackgroundColor(props->backgroundColor);
        } else {
            m_textAreaNode.setBackgroundColor(facebook::react::whiteColor());
            m_textInputNode.setBackgroundColor(facebook::react::whiteColor());
        }
    }
    if (props->textAttributes.alignment) {
        if (!m_props || *(props->textAttributes.alignment) != *(m_props->textAttributes.alignment)) {
            m_textAreaNode.setTextAlign(props->textAttributes.alignment);
            m_textInputNode.setTextAlign(props->textAttributes.alignment);
        }
    }
    if (!m_props || *(props->cursorColor) != *(m_props->cursorColor)) {
        if (props->cursorColor) {
            m_textAreaNode.setCaretColor(props->cursorColor, multiline_);
            m_textInputNode.setCaretColor(props->cursorColor, multiline_);
        } else {
            m_textAreaNode.setCaretColor(facebook::react::blackColor(), multiline_);
            m_textInputNode.setCaretColor(facebook::react::blackColor(), multiline_);
        }
    }
    if (!m_props || props->traits.editable != m_props->traits.editable) {
        m_textAreaNode.setEnabled(props->traits.editable);
        m_textInputNode.setEnabled(props->traits.editable);
    }
    if (!m_props || props->traits.keyboardType != m_props->traits.keyboardType) {
        m_textAreaNode.setInputType(props->traits.keyboardType);
    }
    if (props->maxLength) {
        if (!m_props || props->maxLength != m_props->maxLength) {
            m_textAreaNode.setMaxLength(props->maxLength, multiline_);
            m_textInputNode.setMaxLength(props->maxLength, multiline_);
        }
    }
    if (!m_props || props->placeholder != m_props->placeholder) {
        m_textAreaNode.setPlaceHolder(props->placeholder, multiline_);
        m_textInputNode.setPlaceHolder(props->placeholder, multiline_);
    }
    if (props->placeholderTextColor) {
        if (!m_props || *(props->placeholderTextColor) != *(m_props->placeholderTextColor)) {
            m_textAreaNode.setPlaceHolderColor(props->placeholderTextColor, multiline_);
            m_textInputNode.setPlaceHolderColor(props->placeholderTextColor, multiline_);
        }
    }
    if (props->rawProps.count("focusable") > 0) {
        if (!m_props || props->rawProps["focusable"].asBool() != m_props->rawProps["focusable"].asBool()) {
            m_textAreaNode.setFocusable(props->rawProps["focusable"].asBool());
            m_textInputNode.setFocusable(props->rawProps["focusable"].asBool());
        }
    }
    m_textAreaNode.setId(getTag());
    m_textInputNode.setId(getTag());

    if (!m_props || props->autoFocus != m_props->autoFocus) {
        m_textAreaNode.setAutoFocus(props->autoFocus);
        m_textInputNode.setAutoFocus(props->autoFocus);
    }
    if (!m_props || props->defaultValue != m_props->defaultValue) {
        m_textAreaNode.setTextContent(props->defaultValue);
        m_textInputNode.setTextContent(props->defaultValue);
    }
    if (!m_props || props->text != m_props->text) {
        m_textAreaNode.setTextContent(props->text);
        m_textInputNode.setTextContent(props->text);
    }

    if (!m_props || *(props->selectionColor) != *(m_props->selectionColor)) {
        m_textInputNode.setSelectedBackgroundColor(props->selectionColor);
    }
    if (!m_props || props->traits.secureTextEntry != m_props->traits.secureTextEntry
        || props->traits.keyboardType != m_props->traits.keyboardType) {
        m_textInputNode.setInputType(props->traits.secureTextEntry, props->traits.keyboardType);
    }
    if (!m_props || props->traits.caretHidden != m_props->traits.caretHidden) {
        m_textInputNode.setCaretHidden(props->traits.caretHidden);
    }
    if (!m_props || props->traits.returnKeyType != m_props->traits.returnKeyType) {
        m_textInputNode.setEnterKeyType(props->traits.returnKeyType);
    }
    if (!m_props || props->traits.clearButtonMode != m_props->traits.clearButtonMode) {
        m_textInputNode.setCancelButtonMode(props->traits.clearButtonMode);
    }
}

void TextInputComponentInstance::setLayout(facebook::react::LayoutMetrics layoutMetrics) {
    CppComponentInstance::setLayout(layoutMetrics);
    if (multiline_) {
        m_textInputNode.setPosition(layoutMetrics.frame.origin);
        m_textInputNode.setSize(layoutMetrics.frame.size);
    } else {
        m_textAreaNode.setPosition(layoutMetrics.frame.origin);
        m_textAreaNode.setSize(layoutMetrics.frame.size);
    }
}

void TextInputComponentInstance::handleCommand(std::string const &commandName, folly::dynamic const &args) {
    if(multiline_){
        return;
    }
    if (commandName == "focus") {
        m_textInputNode.setFocusStatus(1);
    }

    if (commandName == "blur") {
        m_textInputNode.setFocusStatus(0);
    }

    if (commandName == "setTextAndSelection" && args.isArray() && args.size() == 4) {
        m_textInputNode.setTextContent(args[1].asString());
        m_textInputNode.setTextSelection(args[2].asInt(), args[3].asInt());
    }
}

ArkUINode &TextInputComponentInstance::getLocalRootArkUINode() {
    if(multiline_){
        return m_textAreaNode;
    }
    return m_textInputNode;
}

} // namespace rnoh
