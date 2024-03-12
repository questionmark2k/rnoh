#include "ScrollViewComponentInstance.h"
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/scrollview/ScrollViewState.h>
#include <react/renderer/components/scrollview/ScrollViewShadowNode.h>


using namespace rnoh;

ScrollViewComponentInstance::ScrollViewComponentInstance(Context context, facebook::react::Tag tag)
    : CppComponentInstance(context, tag) {
    m_scrollNode.insertChild(m_stackNode);
    m_scrollNode.setScrollNodeDelegate(this);
}

ScrollNode &ScrollViewComponentInstance::getLocalRootArkUINode() { return m_scrollNode; }

void ScrollViewComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
    CppComponentInstance::insertChild(childComponentInstance, index);
    m_stackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
};

void ScrollViewComponentInstance::removeChild(ComponentInstance::Shared childComponentInstance) {
    CppComponentInstance::removeChild(childComponentInstance);
    m_stackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
};

void ScrollViewComponentInstance::setLayout(facebook::react::LayoutMetrics layoutMetrics) {
    CppComponentInstance::setLayout(layoutMetrics);
    m_containerSize = layoutMetrics.frame.size;
}

void rnoh::ScrollViewComponentInstance::onStateChanged(SharedConcreteState const &state) {
    auto stateData = state->getData();
    m_stackNode.setSize(stateData.getContentSize());
    m_contentSize = stateData.getContentSize();
}

facebook::react::Point rnoh::ScrollViewComponentInstance::computeChildPoint(facebook::react::Point const &point,
                                                                            TouchTarget::Shared const &child) const {
    auto offset = m_scrollNode.getScrollOffset();
    return CppComponentInstance::computeChildPoint(point + offset, child);
}

void rnoh::ScrollViewComponentInstance::updateStateWithContentOffset(facebook::react::Point contentOffset) {
    if (!m_state) {
        return;
    }
    m_state->updateState([contentOffset](auto const &stateData) {
        auto newData = stateData;
        newData.contentOffset = contentOffset;
        return std::make_shared<facebook::react::ScrollViewShadowNode::ConcreteState::Data const>(newData);
    });
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
    m_eventEmitter->onScroll(scrollViewMetrics);
    updateStateWithContentOffset(scrollViewMetrics.contentOffset);
}