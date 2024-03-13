#include <algorithm>
#include <bits/alltypes.h>
#include "ArkUINode.h"
#include "NativeNodeApi.h"
#include "ArkUINodeRegistry.h"
#include "conversions.h"

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
        ArkUI_NumberValue value[] = {std::max(static_cast<float>(position.x), 0.0f),
                                     std::max(static_cast<float>(position.y), 0.0f)};
        ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_POSITION, &item));
        return *this;
    }

    ArkUINode &ArkUINode::setSize(facebook::react::Size const &size) {
        ArkUI_NumberValue widthValue[] = {static_cast<float>(size.width)};
        ArkUI_AttributeItem widthItem = {widthValue, sizeof(widthValue) / sizeof(ArkUI_NumberValue)};

        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_WIDTH, &widthItem));

        ArkUI_NumberValue heightValue[] = {static_cast<float>(size.height)};
        ArkUI_AttributeItem heightItem = {heightValue, sizeof(heightValue) / sizeof(ArkUI_NumberValue)};

        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_HEIGHT, &heightItem));
        return *this;
    }

    ArkUINode &ArkUINode::setBorderWidth(facebook::react::BorderWidths const &borderWidths) {
        ArkUI_NumberValue borderWidthValue[] = {
            static_cast<float>(borderWidths.top), static_cast<float>(borderWidths.right),
            static_cast<float>(borderWidths.bottom), static_cast<float>(borderWidths.left)};

        ArkUI_AttributeItem borderWidthItem = {borderWidthValue, sizeof(borderWidthValue) / sizeof(ArkUI_NumberValue)};

        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BORDER_WIDTH, &borderWidthItem));
        return *this;
    }

    ArkUINode &ArkUINode::setBorderColor(facebook::react::BorderColors const &borderColors) {
        ArkUI_NumberValue borderColorValue[] = {{.u32 = static_cast<uint32_t>(*borderColors.top)},
                                                {.u32 = static_cast<uint32_t>(*borderColors.right)},
                                                {.u32 = static_cast<uint32_t>(*borderColors.bottom)},
                                                {.u32 = static_cast<uint32_t>(*borderColors.left)}};

        ArkUI_AttributeItem borderColorItem = {borderColorValue, sizeof(borderColorValue) / sizeof(ArkUI_NumberValue)};

        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BORDER_COLOR, &borderColorItem));
        return *this;
    }

    ArkUINode &ArkUINode::setBorderRadius(facebook::react::BorderRadii const &borderRadius) {
        ArkUI_NumberValue borderRadiusValue[] = {
            static_cast<float>(borderRadius.topLeft), static_cast<float>(borderRadius.topRight),
            static_cast<float>(borderRadius.bottomLeft), static_cast<float>(borderRadius.bottomRight)};

        ArkUI_AttributeItem borderRadiusItem = {borderRadiusValue,
                                                sizeof(borderRadiusValue) / sizeof(ArkUI_NumberValue)};

        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BORDER_RADIUS, &borderRadiusItem));
        return *this;
    }

    ArkUINode &ArkUINode::setBorderStyle(facebook::react::BorderStyles const &borderStyles) {
        ArkUI_NumberValue borderStyleValue[] = {
            {.i32 = static_cast<int32_t>(rnoh::convertReactBorderStyleToArk(borderStyles.top))},
            {.i32 = static_cast<int32_t>(rnoh::convertReactBorderStyleToArk(borderStyles.right))},
            {.i32 = static_cast<int32_t>(rnoh::convertReactBorderStyleToArk(borderStyles.bottom))},
            {.i32 = static_cast<int32_t>(rnoh::convertReactBorderStyleToArk(borderStyles.left))}};

        ArkUI_AttributeItem borderStyleItem = {borderStyleValue, sizeof(borderStyleValue) / sizeof(ArkUI_NumberValue)};

        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BORDER_STYLE, &borderStyleItem));
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

    ArkUINode &ArkUINode::transform(facebook::react::Transform const &transform) {
        ArkUI_NumberValue transformCenterValue[] = {0, 0, 0, 0.5f, 0.5f};

        ArkUI_AttributeItem transformCenterItem = {transformCenterValue,
                                                   sizeof(transformCenterValue) / sizeof(ArkUI_NumberValue)};
        maybeThrow(
            NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TRANSFORM_CENTER, &transformCenterItem));

        std::array<ArkUI_NumberValue, 16> transformValue;
        for (int i = 0; i < 16; i++) {
            transformValue[i] = {.f32 = static_cast<float>(transform.matrix[i])};
        }

        ArkUI_AttributeItem transformItem = {transformValue.data(), transformValue.size()};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_TRANSFORM, &transformItem));
        return *this;
    }

    ArkUINode &ArkUINode::setOpacity(facebook::react::Float const &opacity) {
        ArkUI_NumberValue opacityValue[] = {static_cast<float>(opacity)};
        ArkUI_AttributeItem opacityItem = {opacityValue, sizeof(opacityValue) / sizeof(ArkUI_NumberValue)};

        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_OPACITY, &opacityItem));
        return *this;
    }

    ArkUINode::~ArkUINode() {
        if (m_nodeHandle) {
            ArkUINodeRegistry::getInstance().unregisterNode(this);
            NativeNodeApi::getInstance()->disposeNode(m_nodeHandle);
        }
    }

} // namespace rnoh
