#pragma once

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

  int32_t m_nativeEventCount = 0;

  facebook::react::TextInputMetrics getTextInputMetrics();
  facebook::react::TextInputMetrics getTextInputMetrics(std::string text);

  bool m_secureInput{false};

 public:
  TextInputComponentInstance(Context context);

  void onPropsChanged(SharedConcreteProps const& props) override;

  void onStateChanged(SharedConcreteState const& state) override;

  void setLayout(facebook::react::LayoutMetrics layoutMetrics) override;

  void handleCommand(std::string const& commandName, folly::dynamic const& args)
      override;

  void onChange(std::string text) override;

  void onBlur() override;

  void onSubmit() override;

  void onFocus() override;

  ArkUINode& getLocalRootArkUINode() override;
};
} // namespace rnoh