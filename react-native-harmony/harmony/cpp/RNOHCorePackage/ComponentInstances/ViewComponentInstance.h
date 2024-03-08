#pragma once
#include <react/renderer/components/view/ViewEventEmitter.h>
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/StackNode.h"

namespace rnoh {
    class ViewComponentInstance : public CppComponentInstance {
    private:
        StackNode m_stackNode;
        facebook::react::SharedViewEventEmitter m_eventEmitter;

    public:
        ViewComponentInstance(Context context, facebook::react::Tag tag);

        void setEventEmitter(facebook::react::SharedEventEmitter eventEmitter) override;

        facebook::react::SharedViewEventEmitter const &getEventEmitter() const;

        void insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) override;

        void removeChild(ComponentInstance::Shared childComponentInstance) override;

        StackNode &getLocalRootArkUINode() override;

    // TouchTarget implementation
        facebook::react::SharedTouchEventEmitter getTouchEventEmitter() const override;
    };
} // namespace rnoh