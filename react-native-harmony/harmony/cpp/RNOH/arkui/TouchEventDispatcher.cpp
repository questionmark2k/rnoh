#include "TouchEventDispatcher.h"
#include <glog/logging.h>
#include <set>

namespace rnoh {

using Point = facebook::react::Point;

static std::pair<TouchTarget::Shared, Point> findTargetForTouchPoint(
    Point const& point,
    TouchTarget::Shared const& target) {
  bool canHandleTouch = target->canHandleTouch() &&
      target->containsPoint(point) &&
      (target->getTouchEventEmitter() != nullptr);
  bool canChildrenHandleTouch = target->canChildrenHandleTouch() &&
      target->containsPointInBoundingBox(point);

  if (canChildrenHandleTouch) {
    auto children = target->getTouchTargetChildren();
    // we want to check the children in reverse order, since the last child is
    // the topmost one
    std::reverse(children.begin(), children.end());
    for (auto const& child : children) {
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

facebook::react::Touch convertTouchPointToReactTouch(
    ArkUI_NodeTouchPoint const& touchPoint,
    TouchTarget::Shared const& target,
    double timestampSeconds) {
  Point rootPoint{
      .x = static_cast<facebook::react::Float>(touchPoint.nodeX),
      .y = static_cast<facebook::react::Float>(touchPoint.nodeY)};

  Point screenPoint{
      .x = static_cast<facebook::react::Float>(touchPoint.screenX),
      .y = static_cast<facebook::react::Float>(touchPoint.screenY)};

  facebook::react::Touch touch{
      .pagePoint = rootPoint,
      // TODO: calculate offset points
      .offsetPoint = {0, 0},
      .screenPoint = screenPoint,
      .identifier = touchPoint.id,
      .target = target->getTouchTargetTag(),
      .timestamp = timestampSeconds};

  return touch;
}

std::pair<std::unique_ptr<ArkUI_NodeTouchPoint[]>, int32_t> getTouchesFromEvent(
    ArkUI_NodeTouchEvent event) {
  if (event.getTouches == nullptr) {
    // `getTouches` is not implemented -- we assume there's only one active
    // touch
    ArkUI_NodeTouchPoint* touchPoints =
        new ArkUI_NodeTouchPoint[1]{event.actionTouch};
    return std::make_pair(
        std::unique_ptr<ArkUI_NodeTouchPoint[]>(touchPoints), 1);
  }
  ArkUI_NodeTouchPoint* touchPointsPtr;
  auto touchPointsCount = event.getTouches(&touchPointsPtr);
  return std::make_pair(
      std::unique_ptr<ArkUI_NodeTouchPoint[]>(touchPointsPtr),
      touchPointsCount);
}

bool TouchEventDispatcher::canIgnoreMoveEvent(
    facebook::react::TouchEvent currentEvent) {
  if (m_previousEvent.touches.empty()) {
    return false;
  }
  for (auto touch : currentEvent.changedTouches) {
    auto previousTouch = m_previousEvent.touches.find(touch);
    if (previousTouch != m_previousEvent.touches.end()) {
      auto dx = previousTouch->pagePoint.x - touch.pagePoint.x;
      auto dy = previousTouch->pagePoint.y - touch.pagePoint.y;
      if ((dx * dx + dy * dy) > 1) {
        return false;
      }
    } else {
      LOG(ERROR) << "Moved touch with id: " << touch.identifier
                 << " could not be found in previous touch event.";
    }
  }
  return true;
}

void TouchEventDispatcher::dispatchTouchEvent(
    ArkUI_NodeTouchEvent event,
    TouchTarget::Shared const& rootTarget) {
  VLOG(2) << "Touch event received: id=" << event.actionTouch.id
          << ", action type:" << event.action;
  // react-native expects a timestamp in seconds (because rn multiplies the
  // value by 1e3). The timestamp passed by ArkUI is in nanoseconds. We convert
  // it first to miliseconds before casting to lose unnecessary precision. Then
  // we cast it to a double and convert it to seconds.
  double timestampSeconds = static_cast<double>(event.timeStamp / 1e6) / 1e3;

  if (event.action == NODE_ACTION_DOWN) {
    registerTargetForTouch(event.actionTouch, rootTarget);
  } else if (
      m_touchTargetByTouchId.find(event.actionTouch.id) !=
      m_touchTargetByTouchId.end()) {
    Point touchPoint{
        .x = static_cast<facebook::react::Float>(event.actionTouch.nodeX),
        .y = static_cast<facebook::react::Float>(event.actionTouch.nodeY)};
    auto touchTarget = findTargetForTouchPoint(touchPoint, rootTarget).first;
    auto hasCancelled =
        maybeCancelPreviousTouchEvent(timestampSeconds, touchTarget);
    if (hasCancelled) {
      m_touchTargetByTouchId.erase(event.actionTouch.id);
      return;
    }
  }

  auto it = m_touchTargetByTouchId.find(event.actionTouch.id);
  if (it == m_touchTargetByTouchId.end()) {
    VLOG(2) << "No target for current touch event with id: "
            << event.actionTouch.id;
    return;
  }
  auto eventTarget = it->second.lock();
  if (eventTarget == nullptr) {
    LOG(WARNING) << "Target for current touch event has been deleted";
    m_touchTargetByTouchId.erase(it);
    return;
  }

  auto [touchPoints, touchPointsCount] = getTouchesFromEvent(event);

  facebook::react::Touches touches;
  std::optional<facebook::react::Touch> changedTouch;
  facebook::react::Touches targetTouches;

  for (int i = 0; i < touchPointsCount; i++) {
    auto touchPoint = touchPoints[i];
    auto id = touchPoint.id;
    if (m_touchTargetByTouchId.find(id) == m_touchTargetByTouchId.end()) {
      DLOG(INFO) << "Touch with id " << id << " does not exist";
      continue;
    }

    auto touchTarget = m_touchTargetByTouchId[id].lock();
    if (!touchTarget) {
      continue;
    }
    auto touch = convertTouchPointToReactTouch(
        touchPoint, touchTarget, timestampSeconds);
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
    m_touchTargetByTouchId.erase(changedTouch.value().identifier);
  }

  facebook::react::TouchEvent touchEvent{
      .touches = std::move(touches),
      .changedTouches = {changedTouch.value()},
      .targetTouches = std::move(targetTouches)};

  if (event.action == NODE_ACTION_MOVE && canIgnoreMoveEvent(touchEvent)) {
    VLOG(2) << "Should ignore current touchEvent";
    return;
  }
  m_previousEvent = touchEvent;

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

TouchTarget::Shared TouchEventDispatcher::registerTargetForTouch(
    ArkUI_NodeTouchPoint activeTouch,
    TouchTarget::Shared const& rootTarget) {
  auto id = activeTouch.id;
  if (m_touchTargetByTouchId.find(id) != m_touchTargetByTouchId.end()) {
    LOG(ERROR) << "Touch with id " << id << " already exists";
    return nullptr;
  }

  Point touchPoint{
      .x = static_cast<facebook::react::Float>(activeTouch.nodeX),
      .y = static_cast<facebook::react::Float>(activeTouch.nodeY)};
  auto touchTarget = findTargetForTouchPoint(touchPoint, rootTarget).first;
  if (touchTarget) {
    m_touchTargetByTouchId.emplace(activeTouch.id, touchTarget);
    VLOG(2) << "Touch with id " << id << " started on target with tag "
            << touchTarget->getTouchTargetTag();
    return touchTarget;
  }
  return nullptr;
}

bool TouchEventDispatcher::maybeCancelPreviousTouchEvent(
    double timestampInSecs,
    TouchTarget::Shared touchTarget) {
  // check if ancestor is handling touches
  auto shouldEmitTouchCancelEvent = false;
  auto tmpTouchTarget = touchTarget;
  while (tmpTouchTarget) {
    if (tmpTouchTarget->isHandlingTouches()) {
      shouldEmitTouchCancelEvent = true;
      break;
    }
    tmpTouchTarget = tmpTouchTarget->getTouchTargetParent();
  }
  if (!shouldEmitTouchCancelEvent) {
    return false;
  }

  // create new touch event based on the previously emitted event
  auto touchCancelEvent = m_previousEvent;
  touchCancelEvent.targetTouches = {};
  touchCancelEvent.changedTouches = {};
  touchCancelEvent.touches = {};
  for (auto touch : m_previousEvent.touches) {
    touch.timestamp = timestampInSecs;
    touchCancelEvent.changedTouches.insert(touch);
  }

  // emit cancel event
  DLOG(INFO) << "Cancelling previous touch event";
  touchTarget->getTouchEventEmitter()->onTouchCancel(touchCancelEvent);
  return true;
}

} // namespace rnoh
