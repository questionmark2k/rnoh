#include "ComponentInstance.h"
#include <glog/logging.h>

namespace rnoh {
ComponentInstance::ComponentInstance(Context context, facebook::react::Tag tag) : m_tag(tag) {}

void ComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
    auto it = m_children.begin() + index;
    m_children.insert(it, std::move(childComponentInstance));
}

void ComponentInstance::removeChild(ComponentInstance::Shared childComponentInstance) {
    auto it = std::find(m_children.begin(), m_children.end(), childComponentInstance);
    if (it != m_children.end()) {
        m_children.erase(it);
    }
}

void ComponentInstance::setLayout(facebook::react::LayoutMetrics layoutMetrics) {
    this->getLocalRootArkUINode().setPosition(layoutMetrics.frame.origin);
    this->getLocalRootArkUINode().setSize(layoutMetrics.frame.size);
    m_layoutMetrics = layoutMetrics;
}

facebook::react::Point ComponentInstance::computeChildPoint(facebook::react::Point const &point, TouchTarget::Shared const &child) const {
    auto childComponent = std::dynamic_pointer_cast<ComponentInstance const>(child);
    if (!childComponent) {
        LOG(FATAL) << "Child is not a ComponentInstance";
    }

    auto childLayout = childComponent->m_layoutMetrics;

    // TODO: apply inverse transform

    return point - childLayout.frame.origin;
}

bool ComponentInstance::containsPoint(facebook::react::Point const &point) const {
    // TODO: hitslops
    return point.x >= 0 && point.y >= 0 && point.x < m_layoutMetrics.frame.size.width && point.y < m_layoutMetrics.frame.size.height;
}

bool ComponentInstance::containsPointInBoundingBox(facebook::react::Point const &point) const {
    return containsPoint(point);
}

} // namespace rnoh