#include "LoadingProgressNode.h"
#include "NativeNodeApi.h"

namespace rnoh {

LoadingProgressNode::LoadingProgressNode() : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_LOADING_PROGRESS)) {
}

void LoadingProgressNode::setLoadingProgressNodeColor(facebook::react::SharedColor const &color) {

    uint32_t colorValue = *color;
    ArkUI_NumberValue preparedColorValue[] = {{.u32 = colorValue}};
    ArkUI_AttributeItem colorItem = {preparedColorValue, sizeof(preparedColorValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_LOADING_PROGRESS_COLOR, &colorItem));

}

void LoadingProgressNode::setLoadingProgressNodeAnimating(const bool &enable) {
    uint32_t typeValue = 1;
    if (!enable) {
        typeValue = 0;
    }
    ArkUI_NumberValue preparedTypeValue[] = {{.u32 = typeValue}};
    ArkUI_AttributeItem typeItem = {preparedTypeValue, sizeof(preparedTypeValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_LOADING_PROGRESS_ENABLE_LOADING, &typeItem));
}

} // namespace rnoh
