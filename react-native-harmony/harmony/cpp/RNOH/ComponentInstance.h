/**
 * Used only in C-API based Architecture.
 */
#pragma once
#include <vector>
#include <react/renderer/core/LayoutMetrics.h>
#include <react/renderer/core/Props.h>
#include <react/renderer/core/State.h>
#include <react/renderer/core/EventEmitter.h>
#include <react/renderer/core/ReactPrimitives.h>
#include <react/renderer/components/view/ViewProps.h>
#include "RNOH/arkui/ArkUINode.h"
#include "RNOH/TouchTarget.h"

namespace rnoh {

    class ComponentInstance : public TouchTarget {
    private:
    public:
        struct Context {};

        virtual ArkUINode &getLocalRootArkUINode() = 0;

        using Shared = std::shared_ptr<ComponentInstance>;

        ComponentInstance(Context context, facebook::react::Tag tag);

        virtual ~ComponentInstance() = default;

        facebook::react::Tag getTag() const { return m_tag; }

        virtual void insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index);

        virtual void removeChild(ComponentInstance::Shared childComponentInstance);

        virtual void setProps(facebook::react::Props::Shared props) {};

        virtual void setState(facebook::react::State::Shared state) {};

        virtual void setLayout(facebook::react::LayoutMetrics layoutMetrics) {};

        virtual void setEventEmitter(facebook::react::SharedEventEmitter eventEmitter) {};

        virtual void finalizeUpdates() { this->getLocalRootArkUINode().markDirty(); }

        virtual void handleCommand(std::string const &commandName, folly::dynamic const &args) {}

        virtual std::vector<ComponentInstance::Shared> const &getChildren() const { return m_children; }

        // TouchTarget implementation
        facebook::react::Tag getTouchTargetTag() const override { return getTag(); }

        facebook::react::SharedTouchEventEmitter getTouchEventEmitter() const override { return nullptr; }

        virtual std::vector<TouchTarget::Shared> getTouchTargetChildren() const override {
            auto children = getChildren();
            return std::vector<TouchTarget::Shared>(children.begin(), children.end());
        }

    protected:
        facebook::react::Tag m_tag;
        std::vector<ComponentInstance::Shared> m_children;
        facebook::react::LayoutMetrics m_layoutMetrics;
    };

} // namespace rnoh