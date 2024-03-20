#include "TouchEventDispatcher.h"
#include <glog/logging.h>

namespace rnoh {

using Point = facebook::react::Point;

static std::pair<TouchTarget::Shared, Point> findTargetForTouchPoint(Point const &point, TouchTarget::Shared const &target) {
    bool canHandleTouch = target->containsPoint(point) && (target->getTouchEventEmitter() != nullptr);
    bool canChildrenHandleTouch = target->containsPointInBoundingBox(point);

    if (canChildrenHandleTouch) {
        auto children = target->getTouchTargetChildren();
        // we want to check the children in reverse order, since the last child is the topmost one
        std::reverse(children.begin(), children.end());
        for (auto const &child : children) {
            auto childPoint = target->computeChildPoint(point, child);
            auto result = findTargetForTouchPoint(childPoint, child);
            if (result.first != nullptr) {
                return result;
            }
        }
    }

    if (canHandleTouch) {
        return std::make_pair(target, point);
    }

    return std::make_pair(nullptr, Point{});
}

facebook::react::Touch convertTouchPointToReactTouch(ArkUI_NodeTouchPoint const &touchPoint, TouchTarget::Shared const &target, double timestampMillis) {
    Point rootPoint{
        .x = static_cast<facebook::react::Float>(touchPoint.nodeX),
        .y = static_cast<facebook::react::Float>(touchPoint.nodeY)
    };

    Point screenPoint{
        .x = static_cast<facebook::react::Float>(touchPoint.screenX),
        .y = static_cast<facebook::react::Float>(touchPoint.screenY)
    };

    facebook::react::Touch touch{
        .pagePoint = rootPoint,
        // TODO: calculate offset points
        .offsetPoint = {0, 0},
        .screenPoint = screenPoint,
        .identifier = touchPoint.id,
        .target = target->getTouchTargetTag(),
        .timestamp = timestampMillis
    };

    return touch;
}

std::pair<std::unique_ptr<ArkUI_NodeTouchPoint[]>, int32_t> getTouchesFromEvent(ArkUI_NodeTouchEvent event) {
    if (event.getTouches == nullptr) {
        // `getTouches` is not implemented -- we assume there's only one active touch
        ArkUI_NodeTouchPoint *touchPoints = new ArkUI_NodeTouchPoint[1]{event.actionTouch};
        return std::make_pair(std::unique_ptr<ArkUI_NodeTouchPoint[]>(touchPoints), 1);
    }
    ArkUI_NodeTouchPoint *touchPointsPtr;
    auto touchPointsCount = event.getTouches(&touchPointsPtr);
    return std::make_pair(std::unique_ptr<ArkUI_NodeTouchPoint[]>(touchPointsPtr), touchPointsCount);
}

void TouchEventDispatcher::dispatchTouchEvent(ArkUI_NodeTouchEvent event, TouchTarget::Shared const &rootTarget) {
    DLOG(INFO) << "Touch event received: id=" << event.actionTouch.id << ", action type:" << event.action;

    if (event.action == NODE_ACTION_DOWN) {
        registerTargetForTouch(event.actionTouch, rootTarget);
    }

    auto it = m_touchTargetTagById.find(event.actionTouch.id);
    if (it == m_touchTargetTagById.end()) {
        DLOG(INFO) << "No target for current touch event";
        return;
    }
    auto eventTarget = it->second.lock();
    if (eventTarget == nullptr) {
        LOG(WARNING) << "Target for current touch event has been deleted";
        m_touchTargetTagById.erase(it);
        return;
    }

    auto [touchPoints, touchPointsCount] = getTouchesFromEvent(event);

    double timestampMillis = static_cast<double>(event.timeStamp) / 1000;

    facebook::react::Touches touches;
    std::optional<facebook::react::Touch> changedTouch;
    facebook::react::Touches targetTouches;

    for (int i = 0; i < touchPointsCount; i++) {
        auto touchPoint = touchPoints[i];
        auto id = touchPoint.id;
        if (m_touchTargetTagById.find(id) == m_touchTargetTagById.end()) {
            DLOG(INFO) << "Touch with id " << id << " does not exist";
            continue;
        }

        auto touchTarget = m_touchTargetTagById[id].lock();
        if (!touchTarget) {
            continue;
        }
        auto touch = convertTouchPointToReactTouch(touchPoint, touchTarget, timestampMillis);
        touches.insert(touch);
        if (touchTarget->getTouchTargetTag() == eventTarget->getTouchTargetTag()) {
            targetTouches.insert(touch);
        }
        if (id == event.actionTouch.id) {
            changedTouch = touch;
        }
    }

    if (!changedTouch.has_value()) {
        DLOG(INFO) << "No changed touch for current touch event";
        return;
    }

    if (event.action == NODE_ACTION_UP) {
        touches.erase(changedTouch.value());
        targetTouches.erase(changedTouch.value());
        m_touchTargetTagById.erase(changedTouch.value().identifier);
    }

    facebook::react::TouchEvent touchEvent{
        .touches = std::move(touches),
        .changedTouches = {changedTouch.value()},
        .targetTouches = std::move(targetTouches)
    };

    switch (event.action) {
        case NODE_ACTION_DOWN:
            eventTarget->getTouchEventEmitter()->onTouchStart(touchEvent);
            break;
        case NODE_ACTION_MOVE:
            eventTarget->getTouchEventEmitter()->onTouchMove(touchEvent);
            break;
        case NODE_ACTION_UP:
            eventTarget->getTouchEventEmitter()->onTouchEnd(touchEvent);
            break;
        case NODE_ACTION_CANCEL:
        default:
            eventTarget->getTouchEventEmitter()->onTouchCancel(touchEvent);
            break;
    }
}

void TouchEventDispatcher::registerTargetForTouch(ArkUI_NodeTouchPoint activeTouch, TouchTarget::Shared const &rootTarget) {
    auto id = activeTouch.id;
    if (m_touchTargetTagById.find(id) != m_touchTargetTagById.end()) {
        LOG(ERROR) << "Touch with id " << id << " already exists";
        return;
    }

    Point touchPoint{
        .x = static_cast<facebook::react::Float>(activeTouch.nodeX),
        .y = static_cast<facebook::react::Float>(activeTouch.nodeY)
    };
    auto touchTarget = findTargetForTouchPoint(touchPoint, rootTarget).first;
    if (touchTarget) {
        m_touchTargetTagById.emplace(activeTouch.id, touchTarget);
        DLOG(INFO) << "Touch with id " << id << " started on target with tag " << touchTarget->getTouchTargetTag();
    }
}

} // namespace rnoh
