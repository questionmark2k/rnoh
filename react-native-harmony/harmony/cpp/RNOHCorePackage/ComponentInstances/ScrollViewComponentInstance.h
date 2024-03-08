#pragma once
#include <react/renderer/components/scrollview/ScrollViewEventEmitter.h>
#include "RNOH/ComponentInstance.h"
#include "RNOH/arkui/ScrollNode.h"
#include "RNOH/arkui/StackNode.h"

namespace rnoh {
    class ScrollViewComponentInstance : public ComponentInstance, public ScrollNodeDelegate {
    private:
        ScrollNode m_scrollNode;
        StackNode m_stackNode;
        std::shared_ptr<const facebook::react::ScrollViewEventEmitter> m_scrollViewEventEmitter;
        facebook::react::Size m_contentSize;
        facebook::react::Size m_containerSize;

    public:
        ScrollViewComponentInstance(Context context, facebook::react::Tag tag);

        ScrollNode &getLocalRootArkUINode() override;

        void insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) override;

        void removeChild(ComponentInstance::Shared childComponentInstance) override;

        void setLayout(facebook::react::LayoutMetrics layoutMetrics) override;

        void setState(facebook::react::State::Shared state) override;

        void setEventEmitter(facebook::react::SharedEventEmitter eventEmitter) override;

        void onScroll() override;

        // TouchTarget implementation
        facebook::react::Point computeChildPoint(facebook::react::Point const &point,
                                                 TouchTarget::Shared const &child) const override;
    };
} // namespace rnoh