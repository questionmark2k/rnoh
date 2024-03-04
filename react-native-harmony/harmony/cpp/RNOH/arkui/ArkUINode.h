/**
 * Used only in C-API based Architecture.
 */
#pragma once
#include <arkui/native_node.h>
#include <react/renderer/graphics/Rect.h>
#include <react/renderer/graphics/Color.h>
#include <stdexcept>

namespace rnoh {
    class ArkUINode {
    protected:
        ArkUINode(ArkUI_NodeHandle nodeHandle) : m_nodeHandle(nodeHandle) {}
        ArkUINode(): ArkUINode(nullptr) {}

        ArkUINode(const ArkUINode &other) = delete;
        ArkUINode &operator=(const ArkUINode &other) = delete;

        ArkUINode(ArkUINode &&other) noexcept;
        ArkUINode &operator=(ArkUINode &&other) noexcept;

    public:
        ArkUI_NodeHandle getArkUINodeHandle();

        void markDirty();

        virtual ArkUINode &setPosition(facebook::react::Point const &position);
        virtual ArkUINode &setSize(facebook::react::Size const &size);
        virtual ArkUINode &setBackgroundColor(facebook::react::SharedColor const &color);

        virtual ~ArkUINode();

    protected:
        void maybeThrow(int32_t status) {
            // TODO: map status to error message, maybe add a new error type
            if (status != 0) {
                throw std::runtime_error("ArkUINode operation failed");
            }
        }

        ArkUI_NodeHandle m_nodeHandle;
    };
} // namespace rnoh
