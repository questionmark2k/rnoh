#pragma once
#include "RNOH/ComponentInstance.h"
#include "RNOH/arkui/StackNode.h"

namespace rnoh {
    class ViewComponentInstance : public ComponentInstance {
    private:
        StackNode m_stackNode;

    public:
        ViewComponentInstance(Context context, facebook::react::Tag tag);

        void insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) override;

        void removeChild(ComponentInstance::Shared childComponentInstance) override;

        StackNode &getLocalRootArkUINode() override;
    };
} // namespace rnoh