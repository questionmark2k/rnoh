#pragma once

#include "ArkUINode.h"

namespace rnoh {

class TextNode : public ArkUINode {

public:
    TextNode();

    void setTextContent(const std::string &text);
};

} // namespace rnoh