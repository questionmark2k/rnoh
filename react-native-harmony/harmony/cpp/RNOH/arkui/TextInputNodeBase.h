#pragma once

#include "ArkUINode.h"
#include "react/renderer/graphics/Color.h"
#include "react/renderer/components/textinput/TextInputProps.h"

namespace rnoh {

class TextInputNodeBase : public ArkUINode {
protected:
    TextInputNodeBase(ArkUI_NodeType nodeType);

public:
    void setFocusable(bool const &focusable);

    void setAutoFocus(bool const &autoFocus);

    void setResponseRegion(facebook::react::Point const &position,
        facebook::react::Size const &size);

    void setFocusStatus(int const &focus);

    void setTextContent(std::string const &textContent);

    void setTextSelection(int const &start, int const &end);

    void setFontColor(facebook::react::SharedColor const &color);

    void setFont(facebook::react::TextAttributes const &textAttributes, bool multiline);

    void setCaretColor(facebook::react::SharedColor const &color, bool multiline);

    void setEnabled(bool const &enabled);

    void setMaxLength(int const &maxLength, bool multiline);

    void setPlaceHolder(std::string const &placeHodler, bool multiline);

    void setPlaceHolderColor(facebook::react::SharedColor const &color, bool multiline);

    void setTextAlign(std::optional<facebook::react::TextAlignment> const &textAlign);

private:
    ArkUI_NumberValue convertTextAlign(facebook::react::TextAlignment alignment);

    ArkUI_NumberValue convertFontWeight(int fontWeight);
};

} // namespace rnoh