#include "ArkUINode.h"
#include "NativeNodeApi.h"

namespace rnoh {
ArkUINode::ArkUINode(ArkUINode &&other) noexcept : m_nodeHandle(std::move(other.m_nodeHandle)) {
    other.m_nodeHandle = nullptr;
}

ArkUINode &ArkUINode::operator=(ArkUINode &&other) noexcept {
    std::swap(m_nodeHandle, other.m_nodeHandle);
    return *this;
}

ArkUI_NodeHandle ArkUINode::getArkUINodeHandle() {
    return m_nodeHandle;
}

void ArkUINode::markDirty() {
    // TODO: maybe this can be passed as an arg here,
    // and Component Instance can decide which flag to set in each mutation
    NativeNodeApi::getInstance()->markDirty(getArkUINodeHandle(), ArkUI_NodeDirtyFlag::NODE_NEED_RENDER);
    NativeNodeApi::getInstance()->markDirty(getArkUINodeHandle(), ArkUI_NodeDirtyFlag::NODE_NEED_LAYOUT);
    NativeNodeApi::getInstance()->markDirty(getArkUINodeHandle(), ArkUI_NodeDirtyFlag::NODE_NEED_MEASURE);
}

ArkUINode &ArkUINode::setPosition(facebook::react::Point const &position) {
    ArkUI_NumberValue value[] = {(float)position.x, (float)position.y};
    ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};

    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_POSITION, &item));
    return *this;
}

ArkUINode &ArkUINode::setSize(facebook::react::Size const &size) {
    ArkUI_NumberValue widthValue[] = {(float)size.width};
    ArkUI_AttributeItem widthItem = {widthValue, sizeof(widthValue) / sizeof(ArkUI_NumberValue)};

    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_WIDTH, &widthItem));

    ArkUI_NumberValue heightValue[] = {(float)size.height};
    ArkUI_AttributeItem heightItem = {heightValue, sizeof(heightValue) / sizeof(ArkUI_NumberValue)};

    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_HEIGHT, &heightItem));
    return *this;
}

ArkUINode &ArkUINode::setBackgroundColor(facebook::react::SharedColor const &color) {
    // TODO: figure out if we need to update (to a clear value), or return early here
    if (!facebook::react::isColorMeaningful(color)) {
        return *this;
    }
    uint32_t colorValue = *color; // facebook::react::isColorMeaningful(color) ? *color : *facebook::react::clearColor();
    ArkUI_NumberValue preparedColorValue[] = {{.u32 = colorValue}};
    ArkUI_AttributeItem colorItem = {preparedColorValue, sizeof(preparedColorValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BACKGROUND_COLOR, &colorItem));
    return *this;
}

ArkUINode::~ArkUINode() {
}

} // namespace rnoh