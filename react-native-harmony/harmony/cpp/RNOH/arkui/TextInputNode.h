#pragma once

#include "TextInputNodeBase.h"
#include "react/renderer/graphics/Color.h"
#include "react/renderer/components/textinput/TextInputProps.h"

namespace rnoh {

class TextInputNodeDelegate {
    public:
        virtual ~TextInputNodeDelegate() = default;
        virtual void onChange() {};
        virtual void onBlur() {};
        virtual void onFocus() {};
        virtual void onSubmit() {};
        virtual void onPaste() {};
        virtual void onTextSelectionChange() {};
};

class TextInputNode : public TextInputNodeBase {

protected:
    ArkUI_NodeHandle m_childArkUINodeHandle;
    TextInputNodeDelegate *m_textInputNodeDelegate;

public:
    TextInputNode();
    ~TextInputNode();

    facebook::react::Point getTextInputOffset() const;

    void onNodeEvent(ArkUI_NodeEvent *event) override;

    void setTextInputNodeDelegate(TextInputNodeDelegate *textInputNodeDelegate);

    void setTextContent(std::string const &textContent);

    void setTextSelection(int const &start, int const &end);

    void setCaretHidden(bool hidden);

    void setInputType(bool secure, facebook::react::KeyboardType keyboardType);

    void setSelectedBackgroundColor(facebook::react::SharedColor const &color);

    void setPasswordIconVisibility(bool isVisible);

    void setEnterKeyType(facebook::react::ReturnKeyType returnKeyType);

    void setCancelButtonMode(facebook::react::TextInputAccessoryVisibilityMode mode);
private:
    bool m_secureInput{false};
    facebook::react::KeyboardType m_keyboardType{facebook::react::KeyboardType::Default};

    // util methods
    uint32_t convertColorToTranslucent(facebook::react::SharedColor const &color);

    ArkUI_TextInputType convertInputType(facebook::react::KeyboardType keyboardType);

    ArkUI_EnterKeyType convertEnterKeyType(facebook::react::ReturnKeyType returnKeyType);
};

} // namespace rnoh