#include "TextNode.h"
#include "NativeNodeApi.h"

namespace rnoh {

TextNode::TextNode() : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_TEXT)) {
}

void TextNode::setTextContent(const std::string &text) {
    ArkUI_AttributeItem item = {.string = text.c_str()};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_CONTENT, &item));
}
} // namespace rnoh
