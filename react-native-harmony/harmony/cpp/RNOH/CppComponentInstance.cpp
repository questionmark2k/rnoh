#include "CppComponentInstance.h"
#include <glog/logging.h>

namespace rnoh {
    CppComponentInstance::CppComponentInstance(Context context, facebook::react::Tag tag)
        : ComponentInstance(context, tag) {}

    void CppComponentInstance::setProps(facebook::react::Props::Shared props) {
        if (auto p = std::dynamic_pointer_cast<const facebook::react::ViewProps>(props)) {
            this->getLocalRootArkUINode().setBackgroundColor(p->backgroundColor);

            facebook::react::BorderMetrics borderMetrics = p->resolveBorderMetrics(this->m_layoutMetrics);
            this->getLocalRootArkUINode().setBorderWidth(borderMetrics.borderWidths);
            this->getLocalRootArkUINode().setBorderColor(borderMetrics.borderColors);
        }
    }

    void CppComponentInstance::setLayout(facebook::react::LayoutMetrics layoutMetrics) {
        this->getLocalRootArkUINode().setPosition(layoutMetrics.frame.origin);
        this->getLocalRootArkUINode().setSize(layoutMetrics.frame.size);
        m_layoutMetrics = layoutMetrics;
    }

    facebook::react::LayoutMetrics CppComponentInstance::getLayoutMetrics() const {
        return m_layoutMetrics;
    }

    facebook::react::Point CppComponentInstance::computeChildPoint(facebook::react::Point const &point,
                                                                TouchTarget::Shared const &child) const {
        auto childLayout = child->getLayoutMetrics();

        // TODO: apply inverse transform

        return point - childLayout.frame.origin;
    }

    bool CppComponentInstance::containsPoint(facebook::react::Point const &point) const {
        // TODO: hitslops
        return point.x >= 0 && point.y >= 0 && point.x < m_layoutMetrics.frame.size.width &&
               point.y < m_layoutMetrics.frame.size.height;
    }

    bool CppComponentInstance::containsPointInBoundingBox(facebook::react::Point const &point) const {
        return containsPoint(point);
    }

} // namespace rnoh