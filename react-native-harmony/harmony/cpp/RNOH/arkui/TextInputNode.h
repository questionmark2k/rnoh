#pragma once

#include "TextInputNodeBase.h"
#include "react/renderer/components/textinput/TextInputProps.h"
#include "react/renderer/graphics/Color.h"

namespace rnoh {

class TextInputNodeDelegate {
 public:
  virtual ~TextInputNodeDelegate() = default;
  virtual void onChange(ArkUI_NodeEvent* event){};
  virtual void onBlur(ArkUI_NodeEvent* event){};
  virtual void onFocus(ArkUI_NodeEvent* event){};
  virtual void onSubmit(ArkUI_NodeEvent* event){};
  virtual void onPaste(){};
  virtual void onTextSelectionChange(){};
};

class TextInputNode : public TextInputNodeBase {
 protected:
  ArkUI_NodeHandle m_childArkUINodeHandle;
  TextInputNodeDelegate* m_textInputNodeDelegate;

 public:
  TextInputNode();
  ~TextInputNode();

  facebook::react::Point getTextInputOffset() const;

  void onNodeEvent(ArkUI_NodeEvent* event) override;

  void setTextInputNodeDelegate(TextInputNodeDelegate* textInputNodeDelegate);

  void setTextContent(std::string const& textContent);

  void setTextSelection(int32_t start, int32_t end);

  void setCaretHidden(bool hidden);

  void setInputType(ArkUI_TextInputType keyboardType);

  void setSelectedBackgroundColor(facebook::react::SharedColor const& color);

  void setPasswordIconVisibility(bool isVisible);

  void setEnterKeyType(facebook::react::ReturnKeyType returnKeyType);

  void setCancelButtonMode(
      facebook::react::TextInputAccessoryVisibilityMode mode);

  void setFont(facebook::react::TextAttributes const& textAttributes) override;

  void setCaretColor(facebook::react::SharedColor const& color) override;

  void setMaxLength(int32_t maxLength) override;

  void setPlaceholder(std::string const& placeholder) override;

  void setPlaceholderColor(facebook::react::SharedColor const& color) override;

  void resetSelectedBackgroundColor();

  std::string getTextContent() override;
};

} // namespace rnoh