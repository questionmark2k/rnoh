#include "TextInputComponentInstance.h"
#include "RNOH/arkui/conversions.h"
#include "conversions.h"

#include <folly/dynamic.h>
#include <glog/logging.h>
#include <react/renderer/components/textinput/TextInputProps.h>
#include <react/renderer/components/textinput/TextInputState.h>
#include <react/renderer/core/ConcreteState.h>
#include <sstream>
#include <utility>

namespace rnoh {

TextInputComponentInstance::TextInputComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {
  m_textInputNode.setTextInputNodeDelegate(this);
  m_textAreaNode.setTextAreaNodeDelegate(this);
}

void TextInputComponentInstance::onChange(std::string text) {
  this->m_nativeEventCount++;
  this->m_content = std::move(text);
  m_eventEmitter->onChange(getTextInputMetrics());
}

void TextInputComponentInstance::onSubmit() {
  this->m_nativeEventCount++;
  m_eventEmitter->onSubmitEditing(getTextInputMetrics());
}

void TextInputComponentInstance::onBlur() {
  m_eventEmitter->onBlur(getTextInputMetrics());
  this->m_nativeEventCount++;
  m_eventEmitter->onEndEditing(getTextInputMetrics());
}

void TextInputComponentInstance::onFocus() {
  this->m_nativeEventCount++;
  if (this->m_clearTextOnFocus) {
    m_textAreaNode.setTextContent("");
    m_textInputNode.setTextContent("");
  }
  m_eventEmitter->onFocus(getTextInputMetrics());
}

void TextInputComponentInstance::onTextSelectionChange(
    int32_t location,
    int32_t length) {
  this->m_nativeEventCount++;
  this->m_selectionLocation = location;
  this->m_selectionLength = length;
  m_eventEmitter->onSelectionChange(getTextInputMetrics());
}

facebook::react::TextInputMetrics
TextInputComponentInstance::getTextInputMetrics() {
  auto textInputMetrics = facebook::react::TextInputMetrics();
  textInputMetrics.contentOffset = m_multiline
      ? m_textAreaNode.getTextAreaOffset()
      : m_textInputNode.getTextInputOffset();
  textInputMetrics.containerSize = m_layoutMetrics.frame.size;

  textInputMetrics.eventCount = this->m_nativeEventCount;
  textInputMetrics.selectionRange.location = this->m_selectionLocation;
  textInputMetrics.selectionRange.length = this->m_selectionLength;
  textInputMetrics.zoomScale = 1;
  textInputMetrics.text = this->m_content;
  return textInputMetrics;
}

void TextInputComponentInstance::onPropsChanged(
    SharedConcreteProps const& props) {
  m_multiline = props->traits.multiline;
  CppComponentInstance::onPropsChanged(props);
  m_clearTextOnFocus = props->traits.clearTextOnFocus;
  auto canUpdateWithEventCount =
      props->mostRecentEventCount >= this->m_nativeEventCount;

  if (!m_props ||
      *(props->textAttributes.foregroundColor) !=
          *(m_props->textAttributes.foregroundColor)) {
    if (props->textAttributes.foregroundColor) {
      m_textAreaNode.setFontColor(props->textAttributes.foregroundColor);
      m_textInputNode.setFontColor(props->textAttributes.foregroundColor);
    } else {
      m_textAreaNode.setFontColor(facebook::react::blackColor());
      m_textInputNode.setFontColor(facebook::react::blackColor());
    }
  }
  if (!m_props || props->textAttributes != m_props->textAttributes) {
    m_textAreaNode.setFont(props->textAttributes);
    m_textInputNode.setFont(props->textAttributes);
  }
  if (!m_props || *(props->backgroundColor) != *(m_props->backgroundColor)) {
    if (props->backgroundColor) {
      m_textAreaNode.setBackgroundColor(props->backgroundColor);
      m_textInputNode.setBackgroundColor(props->backgroundColor);
    } else {
      m_textAreaNode.setBackgroundColor(facebook::react::clearColor());
      m_textInputNode.setBackgroundColor(facebook::react::clearColor());
    }
  }
  if (props->textAttributes.alignment) {
    if (!m_props ||
        *(props->textAttributes.alignment) !=
            *(m_props->textAttributes.alignment)) {
      m_textAreaNode.setTextAlign(props->textAttributes.alignment);
      m_textInputNode.setTextAlign(props->textAttributes.alignment);
    }
  }
  if (!m_props || *(props->cursorColor) != *(m_props->cursorColor)) {
    if (props->cursorColor) {
      m_textAreaNode.setCaretColor(props->cursorColor);
      m_textInputNode.setCaretColor(props->cursorColor);
    } else {
      m_textAreaNode.setCaretColor(facebook::react::blackColor());
      m_textInputNode.setCaretColor(facebook::react::blackColor());
    }
  }
  if (!m_props || props->traits.editable != m_props->traits.editable) {
    m_textAreaNode.setEnabled(props->traits.editable);
    m_textInputNode.setEnabled(props->traits.editable);
  }
  if (!m_props || props->traits.keyboardType != m_props->traits.keyboardType) {
    m_textAreaNode.setInputType(
        props->traits.secureTextEntry
            ? ARKUI_TEXTINPUT_TYPE_PASSWORD
            : rnoh::convertInputType(props->traits.keyboardType));
  }
  if (props->maxLength != 0) {
    if (!m_props || props->maxLength != m_props->maxLength) {
      m_textAreaNode.setMaxLength(props->maxLength);
      m_textInputNode.setMaxLength(props->maxLength);
    }
  }
  if (!m_props || props->placeholder != m_props->placeholder) {
    m_textAreaNode.setPlaceholder(props->placeholder);
    m_textInputNode.setPlaceholder(props->placeholder);
  }
  if (props->placeholderTextColor) {
    if (!m_props ||
        *(props->placeholderTextColor) != *(m_props->placeholderTextColor)) {
      m_textAreaNode.setPlaceholderColor(props->placeholderTextColor);
      m_textInputNode.setPlaceholderColor(props->placeholderTextColor);
    }
  }
  if (props->rawProps.count("focusable") > 0) {
    if (!m_props ||
        props->rawProps["focusable"].asBool() !=
            m_props->rawProps["focusable"].asBool()) {
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
  if (canUpdateWithEventCount && (!m_props || props->text != m_props->text)) {
    m_textAreaNode.setTextContent(props->text);
    m_textInputNode.setTextContent(props->text);
  }

  if (!m_props || *(props->selectionColor) != *(m_props->selectionColor)) {
    if (props->selectionColor) {
      m_textInputNode.setSelectedBackgroundColor(props->selectionColor);
      if (!props->cursorColor) {
        m_textInputNode.setCaretColor(props->selectionColor);
      }
    } else {
      m_textInputNode.resetSelectedBackgroundColor();
    }
  }
  if (!m_props ||
      props->traits.secureTextEntry != m_props->traits.secureTextEntry ||
      props->traits.keyboardType != m_props->traits.keyboardType) {
    m_textInputNode.setInputType(
        props->traits.secureTextEntry
            ? ARKUI_TEXTINPUT_TYPE_PASSWORD
            : rnoh::convertInputType(props->traits.keyboardType));
  }
  if (!m_props || props->traits.caretHidden != m_props->traits.caretHidden) {
    m_textInputNode.setCaretHidden(props->traits.caretHidden);
  }
  if (!m_props ||
      props->traits.returnKeyType != m_props->traits.returnKeyType) {
    m_textInputNode.setEnterKeyType(props->traits.returnKeyType);
  }
  if (!m_props ||
      props->traits.clearButtonMode != m_props->traits.clearButtonMode) {
    m_textInputNode.setCancelButtonMode(props->traits.clearButtonMode);
  }
  if (!m_props ||
      !(props->yogaStyle.padding() == m_props->yogaStyle.padding())) {
    m_textInputNode.setPadding(resolveEdges(props->yogaStyle.padding()));
    m_textAreaNode.setPadding(resolveEdges(props->yogaStyle.padding()));
  }
}

void TextInputComponentInstance::setLayout(
    facebook::react::LayoutMetrics layoutMetrics) {
  CppComponentInstance::setLayout(layoutMetrics);
  if (m_multiline) {
    m_textInputNode.setPosition(layoutMetrics.frame.origin);
    m_textInputNode.setSize(layoutMetrics.frame.size);
  } else {
    m_textAreaNode.setPosition(layoutMetrics.frame.origin);
    m_textAreaNode.setSize(layoutMetrics.frame.size);
  }
}

void TextInputComponentInstance::handleCommand(
    std::string const& commandName,
    folly::dynamic const& args) {
  if (m_multiline) {
    if (commandName == "focus") {
      m_textAreaNode.setFocusStatus(1);
    }

    if (commandName == "blur") {
      m_textAreaNode.setFocusStatus(0);
    }
    return;
  }
  if (commandName == "focus") {
    m_textInputNode.setFocusStatus(1);
  }

  if (commandName == "blur") {
    m_textInputNode.setFocusStatus(0);
  }

  if (commandName == "setTextAndSelection" && args.isArray() &&
      args.size() == 4 && args[0].asInt() >= m_nativeEventCount) {
    m_textInputNode.setTextContent(args[1].asString());
    if (args[2].asInt() >= 0 && args[3].asInt() >= 0) {
      m_textInputNode.setTextSelection(args[2].asInt(), args[3].asInt());
    }
  }
}

void TextInputComponentInstance::onStateChanged(
    SharedConcreteState const& state) {
  CppComponentInstance::onStateChanged(state);

  if (state->getData().mostRecentEventCount < this->m_nativeEventCount) {
    return;
  }

  std::ostringstream contentStream;
  for (auto const& fragment :
       state->getData().attributedStringBox.getValue().getFragments()) {
    contentStream << fragment.string;
  }
  auto content = contentStream.str();

  if (m_multiline) {
    m_textAreaNode.setTextContent(content);
  } else {
    m_textInputNode.setTextContent(content);
  }
}

ArkUINode& TextInputComponentInstance::getLocalRootArkUINode() {
  if (m_multiline) {
    return m_textAreaNode;
  }
  return m_textInputNode;
}

} // namespace rnoh
