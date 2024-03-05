#include "ScrollViewComponentInstance.h"

using namespace rnoh;

ScrollViewComponentInstance::ScrollViewComponentInstance(Context context, facebook::react::Tag tag)
    : ComponentInstance(context, tag) {
        m_scrollNode.insertChild(m_stackNode);
    }

ScrollNode& ScrollViewComponentInstance::getLocalRootArkUINode() {
    return m_scrollNode;
}

void ScrollViewComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
    m_stackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
};