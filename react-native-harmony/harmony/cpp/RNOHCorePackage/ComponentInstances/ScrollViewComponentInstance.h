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
        ScrollNode m_scrollNode;
        StackNode m_stackNode;
        facebook::react::Size m_contentSize;
        facebook::react::Size m_containerSize;
        facebook::react::ScrollViewMetrics getScrollViewMetrics();

      public:
        ScrollViewComponentInstance(Context context, facebook::react::Tag tag);

        ScrollNode &getLocalRootArkUINode() override;

        void insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) override;

        void removeChild(ComponentInstance::Shared childComponentInstance) override;

        void setLayout(facebook::react::LayoutMetrics layoutMetrics) override;

        void onStateChanged(SharedConcreteState const &state) override;

        void onScroll() override;

        void onScrollStart() override;

        void onScrollStop() override;

        // TouchTarget implementation
        facebook::react::Point computeChildPoint(facebook::react::Point const &point,
                                                 TouchTarget::Shared const &child) const override;
    
    private:
        void updateStateWithContentOffset(facebook::react::Point contentOffset);
    };
} // namespace rnoh