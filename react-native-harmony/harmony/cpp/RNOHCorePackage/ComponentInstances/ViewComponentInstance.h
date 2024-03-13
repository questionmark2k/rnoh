#pragma once
#include <react/renderer/components/view/ViewShadowNode.h>
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/StackNode.h"

namespace rnoh {
    class ViewComponentInstance : public CppComponentInstance<facebook::react::ViewShadowNode> {
    private:
        StackNode m_stackNode;
        facebook::react::SharedViewEventEmitter m_eventEmitter;

    public:
        ViewComponentInstance(Context context);

        void insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) override;

        void removeChild(ComponentInstance::Shared childComponentInstance) override;

        StackNode &getLocalRootArkUINode() override;
    };
} // namespace rnoh