#pragma once
#include "RNOH/ComponentInstance.h"
#include "RNOH/arkui/ScrollNode.h"
#include "RNOH/arkui/StackNode.h"

namespace rnoh {
    class ScrollViewComponentInstance : public ComponentInstance {
    private:
        ScrollNode m_scrollNode;
        StackNode m_stackNode;

    public:
        ScrollViewComponentInstance(Context context, facebook::react::Tag tag);

        ScrollNode &getLocalRootArkUINode() override;

        void insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) override;

        void removeChild(ComponentInstance::Shared childComponentInstance) override;
    };
} // namespace rnoh