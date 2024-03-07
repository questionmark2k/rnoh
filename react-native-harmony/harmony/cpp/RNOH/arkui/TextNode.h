#pragma once

#include "ArkUINode.h"

namespace rnoh {

class TextNode : public ArkUINode {

public:
    TextNode();

    TextNode& setTextContent(const std::string &text);
    TextNode& setFontColor(facebook::react::SharedColor color);
    TextNode& setFontSize(float fontSize);
};

} // namespace rnoh