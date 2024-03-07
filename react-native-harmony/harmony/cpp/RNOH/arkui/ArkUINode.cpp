#include <algorithm>
#include <bits/alltypes.h>

#include "ArkUINode.h"
#include "NativeNodeApi.h"
#include "ArkUINodeRegistry.h"

namespace rnoh {

    ArkUINode::ArkUINode(ArkUI_NodeHandle nodeHandle) : m_nodeHandle(nodeHandle) {
        ArkUINodeRegistry::getInstance().registerNode(this);
    }

    ArkUINode::ArkUINode(ArkUINode &&other) noexcept : m_nodeHandle(std::move(other.m_nodeHandle)) {
        other.m_nodeHandle = nullptr;
    }

    ArkUINode &ArkUINode::operator=(ArkUINode &&other) noexcept {
        std::swap(m_nodeHandle, other.m_nodeHandle);
        return *this;
    }

    void ArkUINode::onNodeEvent(ArkUI_NodeEvent *event) {}

    ArkUI_NodeHandle ArkUINode::getArkUINodeHandle() { return m_nodeHandle; }

    void ArkUINode::markDirty() {
        // TODO: maybe this can be passed as an arg here,
        // and Component Instance can decide which flag to set in each mutation
        NativeNodeApi::getInstance()->markDirty(getArkUINodeHandle(), ArkUI_NodeDirtyFlag::NODE_NEED_RENDER);
        NativeNodeApi::getInstance()->markDirty(getArkUINodeHandle(), ArkUI_NodeDirtyFlag::NODE_NEED_LAYOUT);
        NativeNodeApi::getInstance()->markDirty(getArkUINodeHandle(), ArkUI_NodeDirtyFlag::NODE_NEED_MEASURE);
    }

    ArkUINode &ArkUINode::setPosition(facebook::react::Point const &position) {
        if (position.x < 0 || position.y < 0) {
            LOG(ERROR) << "ArkUI doesn't support negative coordinates when setting a position (x: " << position.x
                       << ", y: " << position.y << ").";
        }
        ArkUI_NumberValue value[] = {std::max((float)position.x, 0.0f), std::max((float)position.y, 0.0f)};
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

    ArkUINode &ArkUINode::setBorderWidth(facebook::react::BorderWidths const &borderWidths) {

        ArkUI_NumberValue borderWidthValue[] = {(float)borderWidths.top, (float)borderWidths.right,
                                                (float)borderWidths.bottom, (float)borderWidths.left};

        ArkUI_AttributeItem borderWidthItem = {borderWidthValue, sizeof(borderWidthValue) / sizeof(ArkUI_NumberValue)};

        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BORDER_WIDTH, &borderWidthItem));
        return *this;
    }

    ArkUINode &ArkUINode::setBorderColor(facebook::react::BorderColors const &borderColors) {

        ArkUI_NumberValue borderColorValue[] = {{.u32 = (uint32_t)*borderColors.top},
                                                {.u32 = (uint32_t)*borderColors.right},
                                                {.u32 = (uint32_t)*borderColors.bottom},
                                                {.u32 = (uint32_t)*borderColors.left}};

        ArkUI_AttributeItem borderColorItem = {borderColorValue, sizeof(borderColorValue) / sizeof(ArkUI_NumberValue)};

        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BORDER_COLOR, &borderColorItem));
        return *this;
    }

    ArkUINode &ArkUINode::setBackgroundColor(facebook::react::SharedColor const &color) {
        // TODO: figure out if we need to update (to a clear value), or return early here
        if (!facebook::react::isColorMeaningful(color)) {
            return *this;
        }
        uint32_t colorValue =
            *color; // facebook::react::isColorMeaningful(color) ? *color : *facebook::react::clearColor();
        ArkUI_NumberValue preparedColorValue[] = {{.u32 = colorValue}};
        ArkUI_AttributeItem colorItem = {preparedColorValue, sizeof(preparedColorValue) / sizeof(ArkUI_NumberValue)};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BACKGROUND_COLOR, &colorItem));
        return *this;
    }

    ArkUINode::~ArkUINode() {
        if (m_nodeHandle) {
            ArkUINodeRegistry::getInstance().unregisterNode(this);
            NativeNodeApi::getInstance()->disposeNode(m_nodeHandle);
        }
    }

} // namespace rnoh
