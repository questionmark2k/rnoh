/**
 * Used only in C-API based Architecture.
 */
#pragma once
#include <vector>
#include <react/renderer/core/LayoutMetrics.h>
#include <react/renderer/core/Props.h>
#include <react/renderer/core/State.h>
#include <react/renderer/core/ReactPrimitives.h>
#include <react/renderer/components/view/ViewProps.h>
#include "RNOH/arkui/ArkUINode.h"


namespace rnoh {

    class ComponentInstance {
    private:
    protected:
        facebook::react::Tag m_tag;


    public:
        struct Context {};

        virtual ArkUINode &getLocalRootArkUINode() = 0;

        using Shared = std::shared_ptr<ComponentInstance>;

        ComponentInstance(Context context, facebook::react::Tag tag) : m_tag(tag) {}

        virtual ~ComponentInstance() {}

        facebook::react::Tag getTag() { return m_tag; }

        virtual void insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {}

        virtual void removeChild(facebook::react::Tag tag) {}

        virtual void setProps(facebook::react::Props::Shared props) {
            if (auto p = std::dynamic_pointer_cast<const facebook::react::ViewProps>(props)) {
                this->getLocalRootArkUINode().setBackgroundColor(p->backgroundColor);
            }
        }

        virtual void setState(facebook::react::State::Shared state) {}

        virtual void setLayout(facebook::react::LayoutMetrics layoutMetrics) {
            this->getLocalRootArkUINode().setPosition(layoutMetrics.frame.origin);
            this->getLocalRootArkUINode().setSize(layoutMetrics.frame.size);
        }

        virtual void finalizeUpdates() { this->getLocalRootArkUINode().markDirty(); }

        virtual void handleCommand(std::string const &commandName, folly::dynamic const &args) {}
    };

} // namespace rnoh