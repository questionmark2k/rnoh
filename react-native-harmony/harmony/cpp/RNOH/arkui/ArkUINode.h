/**
 * Used only in C-API based Architecture.
 */
#pragma once
#include <arkui/native_node.h>
#include <react/renderer/graphics/Rect.h>
#include <react/renderer/graphics/Color.h>
#include <stdexcept>
#include "glog/logging.h"
#include "react/renderer/components/view/primitives.h"
#include "react/renderer/graphics/Transform.h"

namespace rnoh {
    class ArkUINode {
    protected:
        ArkUINode(ArkUI_NodeHandle nodeHandle);

        ArkUINode(const ArkUINode &other) = delete;
        ArkUINode &operator=(const ArkUINode &other) = delete;

        ArkUINode(ArkUINode &&other) noexcept;
        ArkUINode &operator=(ArkUINode &&other) noexcept;

    public:
        ArkUI_NodeHandle getArkUINodeHandle();

        void markDirty();

        virtual ArkUINode &setPosition(facebook::react::Point const &position);
        virtual ArkUINode &setSize(facebook::react::Size const &size);
        virtual ArkUINode &setBorderWidth(facebook::react::BorderWidths const &borderWidths);
        virtual ArkUINode &setBorderColor(facebook::react::BorderColors const &borderColors);
        virtual ArkUINode &setBorderRadius(facebook::react::BorderRadii const &borderRadius);
        virtual ArkUINode &setBorderStyle(facebook::react::BorderStyles const &borderStyles);
        virtual ArkUINode &setBackgroundColor(facebook::react::SharedColor const &color);
        virtual ArkUINode &transform(facebook::react::Transform const &transform);
        virtual ArkUINode &setOpacity(facebook::react::Float const &opacity);

        virtual void onNodeEvent(ArkUI_NodeEvent *event);

        virtual ~ArkUINode();

    protected:
        void maybeThrow(int32_t status) {
            // TODO: map status to error message, maybe add a new error type
            if (status != 0) {
                throw std::runtime_error(std::string("ArkUINode operation failed with status: ") +
                                         std::to_string(status));
            }
        }

        ArkUI_NodeHandle m_nodeHandle;
    };
} // namespace rnoh
