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

        void setState(facebook::react::State::Shared state) override;

    // TouchTarget implementation
        facebook::react::Point computeChildPoint(facebook::react::Point const &point, TouchTarget::Shared const &child) const override;
    };
} // namespace rnoh