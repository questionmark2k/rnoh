#pragma once

#include <react/renderer/graphics/Point.h>
#include <react/renderer/components/view/TouchEventEmitter.h>

namespace rnoh {
class TouchTarget {
    using Point = facebook::react::Point;

  public:
    using Shared = std::shared_ptr<const TouchTarget>;
    using Weak = std::weak_ptr<const TouchTarget>;

    virtual Point computeChildPoint(Point const &point, TouchTarget::Shared const &child) const = 0;
    virtual bool containsPoint(Point const &point) const = 0;
    virtual bool containsPointInBoundingBox(Point const &point) const = 0;
    virtual facebook::react::Tag getTouchTargetTag() const = 0;
    virtual facebook::react::SharedTouchEventEmitter getTouchEventEmitter() const = 0;
    virtual std::vector<Shared> getTouchTargetChildren() const = 0;
    virtual facebook::react::LayoutMetrics getLayoutMetrics() const = 0;
};
} // namespace rnoh