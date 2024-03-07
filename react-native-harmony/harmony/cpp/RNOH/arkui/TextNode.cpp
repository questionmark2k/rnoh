#include "TextNode.h"
#include "NativeNodeApi.h"
#include "glog/logging.h"

using namespace rnoh;

TextNode::TextNode() : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_TEXT)) {}

TextNode &TextNode::setTextContent(const std::string &text) {
    ArkUI_AttributeItem item = {.string = text.c_str()};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TEXT_CONTENT, &item));
    return *this;
}

TextNode &TextNode::setFontColor(facebook::react::SharedColor color) {
    if (!facebook::react::isColorMeaningful(color)) {
        return *this;
    }
    uint32_t colorValue =
        *color; // facebook::react::isColorMeaningful(color) ? *color : *facebook::react::clearColor();
    ArkUI_NumberValue preparedColorValue[] = {{.u32 = colorValue}};
    ArkUI_AttributeItem colorItem = {preparedColorValue, sizeof(preparedColorValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_FONT_COLOR, &colorItem));
    return *this;
}

TextNode &TextNode::setFontSize(float fontSize) {
    ArkUI_NumberValue value[] = {fontSize};
    ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_FONT_SIZE, &item));
    return *this;
}
