#pragma once

#include <react/renderer/graphics/Point.h>
#include <react/renderer/graphics/Transform.h>
#include <react/renderer/components/view/TouchEventEmitter.h>

namespace rnoh {
class TouchTarget {
    using Point = facebook::react::Point;

  public:
    using Shared = std::shared_ptr<const TouchTarget>;
    using Weak = std::weak_ptr<const TouchTarget>;

    virtual Point computeChildPoint(Point const &point, TouchTarget::Shared const &child) const;
    virtual bool containsPoint(Point const &point) const = 0;
    virtual bool containsPointInBoundingBox(Point const &point) const = 0;
    virtual bool canHandleTouch() const = 0;
    virtual bool canChildrenHandleTouch() const = 0;
    /**
     If a component is designed to handle touch events (e.g., Scroll), it should return true if handling is currently
     in progress. This is done to prevent touches from being mistakenly interpreted as taps, particularly during
     scrolling activities.
     */
    virtual bool isHandlingTouches() const { 
        return false;
    };
    virtual facebook::react::Tag getTouchTargetTag() const = 0;
    virtual facebook::react::SharedTouchEventEmitter getTouchEventEmitter() const = 0;
    virtual std::vector<Shared> getTouchTargetChildren() const = 0;
    virtual facebook::react::LayoutMetrics getLayoutMetrics() const = 0;
    virtual facebook::react::Transform getTransform() const = 0;
    virtual TouchTarget::Shared getTouchTargetParent() const = 0;
};
} // namespace rnoh