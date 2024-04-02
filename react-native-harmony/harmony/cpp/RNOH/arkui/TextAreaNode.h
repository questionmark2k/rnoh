#pragma once

#include "ArkUINode.h"
#include "TextInputNodeBase.h"
#include "react/renderer/graphics/Color.h"
#include "react/renderer/components/textinput/TextInputProps.h"
#include "react/renderer/components/textinput/TextInputEventEmitter.h"

namespace rnoh {

    class TextAreaNodeDelegate {
    public:
        virtual ~TextAreaNodeDelegate() = default;
        virtual void onChange(ArkUI_NodeEvent *event){};
        virtual void onBlur(ArkUI_NodeEvent *event){};
        virtual void onFocus(ArkUI_NodeEvent *event){};
        virtual void onPaste(){};
        virtual void onTextSelectionChange(){};
    };

    class TextAreaNode : public TextInputNodeBase {
    protected:
        ArkUI_NodeHandle m_childArkUINodeHandle;
        TextAreaNodeDelegate *m_textAreaNodeDelegate;

    public:
        TextAreaNode();
        ~TextAreaNode() override;

        facebook::react::Point getTextAreaOffset() const;

        void onNodeEvent(ArkUI_NodeEvent *event) override;

        void setTextAreaNodeDelegate(TextAreaNodeDelegate *textAreaNodeDelegate);

        void setTextContent(std::string const &textContent);

        void setInputType(facebook::react::KeyboardType keyboardType);

        void setFont(facebook::react::TextAttributes const &textAttributes) override;

        void setCaretColor(facebook::react::SharedColor const &color) override;

        void setMaxLength(int32_t maxLength) override;

        void setPlaceholder(std::string const &placeholder) override;

        void setPlaceholderColor(facebook::react::SharedColor const &color) override;

        void defaultSetPadding();

        std::string getTextContent() override;
    };

} // namespace rnoh