#pragma once

#include <bits/alltypes.h>
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/TextAreaNode.h"
#include "RNOH/arkui/TextInputNode.h"
#include "react/renderer/components/textinput/TextInputEventEmitter.h"
#include "react/renderer/components/textinput/TextInputShadowNode.h"

namespace rnoh {
class TextInputComponentInstance
    : public CppComponentInstance<facebook::react::TextInputShadowNode>,
      public TextInputNodeDelegate,
      public TextAreaNodeDelegate {
 private:
  TextInputNode m_textInputNode;
  TextAreaNode m_textAreaNode;
  bool m_multiline{false};

  facebook::react::TextInputMetrics getTextInputMetrics(ArkUI_NodeEvent* event);

  bool m_secureInput{false};
    
  bool m_clearTextOnFocus{false};
    
  std::string m_content = "";
    
  int32_t m_nativeEventCount = 0;

 public:
  TextInputComponentInstance(Context context);

  void onPropsChanged(SharedConcreteProps const& props) override;

  void onStateChanged(SharedConcreteState const& state) override;

  void setLayout(facebook::react::LayoutMetrics layoutMetrics) override;

  void handleCommand(std::string const& commandName, folly::dynamic const& args)
      override;

  void onChange(ArkUI_NodeEvent* event) override;

  void onBlur(ArkUI_NodeEvent* event) override;

  void onSubmit(ArkUI_NodeEvent* event) override;

  void onFocus(ArkUI_NodeEvent* event) override;

  ArkUINode& getLocalRootArkUINode() override;
};
} // namespace rnoh