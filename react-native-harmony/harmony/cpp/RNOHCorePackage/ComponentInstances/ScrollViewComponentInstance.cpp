#include "ScrollViewComponentInstance.h"
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/scrollview/ScrollViewState.h>


using namespace rnoh;

ScrollViewComponentInstance::ScrollViewComponentInstance(Context context, facebook::react::Tag tag)
    : ComponentInstance(context, tag) {
        m_scrollNode.insertChild(m_stackNode);
    }

ScrollNode& ScrollViewComponentInstance::getLocalRootArkUINode() {
    return m_scrollNode;
}

void ScrollViewComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
    ComponentInstance::insertChild(childComponentInstance, index);
    m_stackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
};

void ScrollViewComponentInstance::removeChild(ComponentInstance::Shared childComponentInstance) {
    ComponentInstance::removeChild(childComponentInstance);
    m_stackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
};

void ScrollViewComponentInstance::setState(facebook::react::State::Shared state) {
    ComponentInstance::setState(state);
    auto scrollViewState =
        std::dynamic_pointer_cast<const facebook::react::ConcreteState<facebook::react::ScrollViewState>>(state);
    if (scrollViewState == nullptr) {
        return;
    }
    m_stackNode.setSize(scrollViewState->getData().getContentSize()); 
}

facebook::react::Point rnoh::ScrollViewComponentInstance::computeChildPoint(facebook::react::Point const &point, TouchTarget::Shared const &child) const {
    auto offset = m_scrollNode.getScrollOffset();
    return ComponentInstance::computeChildPoint(point + offset, child);
};
