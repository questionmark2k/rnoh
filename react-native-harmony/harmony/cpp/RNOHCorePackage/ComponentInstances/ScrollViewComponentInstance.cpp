#include "ScrollViewComponentInstance.h"
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/scrollview/ScrollViewState.h>


using namespace rnoh;

ScrollViewComponentInstance::ScrollViewComponentInstance(Context context, facebook::react::Tag tag)
    : ComponentInstance(context, tag) {
    m_scrollNode.insertChild(m_stackNode);
    m_scrollNode.setScrollNodeDelegate(this);
}

ScrollNode &ScrollViewComponentInstance::getLocalRootArkUINode() { return m_scrollNode; }

void ScrollViewComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
    ComponentInstance::insertChild(childComponentInstance, index);
    m_stackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
};

void ScrollViewComponentInstance::removeChild(ComponentInstance::Shared childComponentInstance) {
    ComponentInstance::removeChild(childComponentInstance);
    m_stackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
};

void ScrollViewComponentInstance::setLayout(facebook::react::LayoutMetrics layoutMetrics) {
    m_containerSize = layoutMetrics.frame.size;
}

void ScrollViewComponentInstance::setState(facebook::react::State::Shared state) {
    ComponentInstance::setState(state);
    auto scrollViewState =
        std::dynamic_pointer_cast<const facebook::react::ConcreteState<facebook::react::ScrollViewState>>(state);
    if (scrollViewState == nullptr) {
        return;
    }
    auto stateData = scrollViewState->getData();
    m_stackNode.setSize(stateData.getContentSize());
    m_contentSize = stateData.getContentSize();
}

facebook::react::Point rnoh::ScrollViewComponentInstance::computeChildPoint(facebook::react::Point const &point,
                                                                            TouchTarget::Shared const &child) const {
    auto offset = m_scrollNode.getScrollOffset();
    return ComponentInstance::computeChildPoint(point + offset, child);
};

void ScrollViewComponentInstance::setEventEmitter(facebook::react::SharedEventEmitter eventEmitter) {
    ComponentInstance::setEventEmitter(eventEmitter);
    auto scrollViewEventEmitter =
        std::dynamic_pointer_cast<const facebook::react::ScrollViewEventEmitter>(eventEmitter);
    if (scrollViewEventEmitter == nullptr) {
        return;
    }
    m_scrollViewEventEmitter = scrollViewEventEmitter;
}


void ScrollViewComponentInstance::onScroll() {
    auto scrollViewMetrics = facebook::react::ScrollViewMetrics();
    scrollViewMetrics.responderIgnoreScroll = true;
    scrollViewMetrics.zoomScale = 1;
    scrollViewMetrics.contentSize = m_contentSize;
    scrollViewMetrics.contentOffset = m_scrollNode.getScrollOffset();
    scrollViewMetrics.containerSize = m_containerSize;
    DLOG(INFO) << "onScroll (contentOffset: " << scrollViewMetrics.contentOffset.x << ", "
               << scrollViewMetrics.contentOffset.y << "; contentSize: " << scrollViewMetrics.contentSize.width
               << ", " << scrollViewMetrics.contentSize.height
               << "; containerSize: " << scrollViewMetrics.containerSize.width << ", "
               << scrollViewMetrics.containerSize.height << ")";
    m_scrollViewEventEmitter->onScroll(scrollViewMetrics);
}