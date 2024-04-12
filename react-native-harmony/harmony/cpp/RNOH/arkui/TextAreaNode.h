#pragma once

#include "ArkUINode.h"
#include "TextInputNodeBase.h"
#include "react/renderer/components/textinput/TextInputEventEmitter.h"
#include "react/renderer/components/textinput/TextInputProps.h"
#include "react/renderer/graphics/Color.h"

namespace rnoh {

class TextAreaNodeDelegate {
 public:
  virtual ~TextAreaNodeDelegate() = default;
  virtual void onChange(std::string text){};
  virtual void onBlur(){};
  virtual void onFocus(){};
  virtual void onPaste(){};
  virtual void onTextSelectionChange(){};
};

class TextAreaNode : public TextInputNodeBase {
 protected:
  TextAreaNodeDelegate* m_textAreaNodeDelegate;

 public:
  TextAreaNode();
  ~TextAreaNode() override;

  facebook::react::Point getTextAreaOffset() const;

  void onNodeEvent(ArkUI_NodeEventType eventType, EventArgs& eventArgs)
      override;

  void onNodeEvent(ArkUI_NodeEventType eventType, std::string_view eventString)
      override;

  void setTextAreaNodeDelegate(TextAreaNodeDelegate* textAreaNodeDelegate);

  void setTextContent(std::string const& textContent);

  void setInputType(ArkUI_TextInputType keyboardType);

  void setFont(facebook::react::TextAttributes const& textAttributes) override;

  void setCaretColor(facebook::react::SharedColor const& color) override;

  void setMaxLength(int32_t maxLength) override;

  void setPlaceholder(std::string const& placeholder) override;

  void setPlaceholderColor(facebook::react::SharedColor const& color) override;

  void defaultSetPadding();

  std::string getTextContent() override;
};

} // namespace rnoh