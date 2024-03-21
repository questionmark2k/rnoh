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
        virtual void onChange() {};
        virtual void onBlur() {};
        virtual void onFocus() {};
        virtual void onPaste() {};
        virtual void onTextSelectionChange() {};
};

class TextAreaNode : public TextInputNodeBase {
protected:
    ArkUI_NodeHandle m_childArkUINodeHandle;
    TextAreaNodeDelegate *m_textAreaNodeDelegate;

public:
    TextAreaNode();
    ~TextAreaNode();

    facebook::react::Point getTextAreaOffset() const;

    void onNodeEvent(ArkUI_NodeEvent *event) override;

    void setTextAreaNodeDelegate(TextAreaNodeDelegate *textAreaNodeDelegate);

    void setTextContent(std::string const &textContent);

    void setTextSelection(int const &start, int const &end);

    void setInputType(facebook::react::KeyboardType keyboardType);

    ArkUI_TextAreaType convertTextAreaInputType(facebook::react::KeyboardType keyboardType);
private:
    facebook::react::KeyboardType keyboardType_{facebook::react::KeyboardType::Default};
};

} // namespace rnoh