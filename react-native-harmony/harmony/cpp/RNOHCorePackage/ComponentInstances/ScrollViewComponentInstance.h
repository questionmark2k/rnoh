#pragma once
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/ScrollNode.h"
#include "RNOH/arkui/StackNode.h"
#include <react/renderer/components/scrollview/ScrollViewEventEmitter.h>
#include <react/renderer/components/scrollview/ScrollViewShadowNode.h>


namespace rnoh {

    class ScrollViewComponentInstance 
        : public CppComponentInstance<facebook::react::ScrollViewShadowNode>,
          public ScrollNodeDelegate {
    private:
        enum ScrollState : int32_t { IDLE, SCROLL, FLING };
        ScrollNode m_scrollNode;
        StackNode m_stackNode;
        facebook::react::Size m_contentSize;
        facebook::react::Size m_containerSize;
        ScrollState m_scrollState;
        facebook::react::ScrollViewMetrics getScrollViewMetrics();
        facebook::react::Float getFrictionFromDecelerationRate(facebook::react::Float decelerationRate);
        void emitOnScrollEndDragEvent();
        void emitOnMomentumScrollEndEvent();

    public:
        ScrollViewComponentInstance(Context context);

        ScrollNode &getLocalRootArkUINode() override;

        void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
        void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;

        void setLayout(facebook::react::LayoutMetrics layoutMetrics) override;

        void onStateChanged(SharedConcreteState const &state) override;

        void onPropsChanged(SharedConcreteProps const &props) override;

        // ScrollNodeDelegate implementation
        void onScroll() override;
        void onScrollStart() override;
        void onScrollStop() override;
        float onScrollFrameBegin(float offset, int32_t scrollState) override;

        // TouchTarget implementation
        facebook::react::Point computeChildPoint(facebook::react::Point const &point,
                                                 TouchTarget::Shared const &child) const override;
    
    private:
        void updateStateWithContentOffset(facebook::react::Point contentOffset);
    };
} // namespace rnoh