#pragma once

#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/TextAreaNode.h"
#include "RNOH/arkui/TextInputNode.h"
#include "react/renderer/components/textinput/TextInputEventEmitter.h"
#include "react/renderer/components/textinput/TextInputShadowNode.h"

namespace rnoh {
    class TextInputComponentInstance : public CppComponentInstance<facebook::react::TextInputShadowNode>,
        public TextInputNodeDelegate, public TextAreaNodeDelegate {
    private:
        TextInputNode m_textInputNode;
        TextAreaNode m_textAreaNode;
        bool multiline_ = false;
        
        facebook::react::TextInputMetrics getTextInputMetrics();
    public:
        TextInputComponentInstance(Context context);

        void onPropsChanged(std::shared_ptr<const facebook::react::TextInputProps> const &props) override;

        void setLayout(facebook::react::LayoutMetrics layoutMetrics) override;

        void handleCommand(std::string const &commandName, folly::dynamic const &args) override;
        
        void onChange() override;

        void onBlur() override;

        void onSubmit() override;

        void onFocus() override;

        ArkUINode &getLocalRootArkUINode() override;
    };
} // namespace rnoh