#pragma once
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/ScrollNode.h"
#include "RNOH/arkui/StackNode.h"
#include <react/renderer/components/scrollview/ScrollViewEventEmitter.h>
#include <react/renderer/components/scrollview/ScrollViewShadowNode.h>

namespace rnoh {

    class ScrollViewComponentInstance : public CppComponentInstance<facebook::react::ScrollViewShadowNode>,
                                        public ScrollNodeDelegate {
    private:
        enum ScrollState : int32_t { IDLE, SCROLL, FLING };
        ScrollNode m_scrollNode;
        StackNode m_stackNode;
        facebook::react::Size m_contentSize;
        facebook::react::Size m_containerSize;
        ScrollState m_scrollState;
        facebook::react::Point m_currentOffset = {0, 0};
        bool m_persistentScrollbar = false;
        long long m_lastScrollDispatchTime = 0;
        bool m_allowNextScrollEvent = false;
        facebook::react::Float m_scrollEventThrottle = 0;
        facebook::react::ScrollViewMetrics getScrollViewMetrics();
        facebook::react::Float getFrictionFromDecelerationRate(facebook::react::Float decelerationRate);
        void emitOnScrollEndDragEvent();
        void emitOnMomentumScrollEndEvent();
        void scrollToEnd(bool animated);
        ArkUI_ScrollBarDisplayMode getScrollBarDisplayMode(bool horizontal, bool persistentScrollBar,
                                                           bool showsVerticalScrollIndicator,
                                                           bool showsHorizontalScrollIndicator);
        void setScrollSnap(bool snapToStart, bool snapToEnd, const std::vector<facebook::react::Float> &snapToOffsets,
                           facebook::react::Float snapToInterval,
                           facebook::react::ScrollViewSnapToAlignment snapToAlignment);
        bool scrollMovedBySignificantOffset(facebook::react::Point newOffset);

      public:
        ScrollViewComponentInstance(Context context);

        ScrollNode &getLocalRootArkUINode() override;

        void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
        void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;

        void setLayout(facebook::react::LayoutMetrics layoutMetrics) override;

        void onStateChanged(SharedConcreteState const &state) override;
        void onPropsChanged(SharedConcreteProps const &props) override;

        void handleCommand(std::string const &commandName, folly::dynamic const &args) override;

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