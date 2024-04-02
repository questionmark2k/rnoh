#pragma once
#include <react/renderer/components/view/ViewShadowNode.h>
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/StackNode.h"

namespace rnoh {
    class ViewComponentInstance : public CppComponentInstance<facebook::react::ViewShadowNode>,
        public StackNodeDelegate {
    private:
        StackNode m_stackNode;

    public:
        ViewComponentInstance(Context context);

        void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
        void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;

        void onClick() override;
        StackNode &getLocalRootArkUINode() override;
    };
} // namespace rnoh