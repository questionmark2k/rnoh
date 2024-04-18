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
    TouchPoint const& touchPoint,
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

TouchPoint getActiveTouchFromEvent(ArkUI_UIInputEvent* event) {
  TouchPoint actionTouch{};
#ifdef C_API_ARCH
  actionTouch = TouchPoint{
      .id = OH_ArkUI_PointerEvent_GetPointerId(event, 0),
      .nodeX = int32_t(OH_ArkUI_PointerEvent_GetX(event)),
      .nodeY = int32_t(OH_ArkUI_PointerEvent_GetY(event)),
      .screenX = int32_t(OH_ArkUI_PointerEvent_GetDisplayX(event)),
      .screenY = int32_t(OH_ArkUI_PointerEvent_GetDisplayY(event))};
#endif
  return actionTouch;
}

std::vector<TouchPoint> getTouchesFromEvent(ArkUI_UIInputEvent* event) {
  std::vector<TouchPoint> result;
#ifdef C_API_ARCH
  auto touchPointCount = OH_ArkUI_PointerEvent_GetPointerCount(event);
  result.reserve(touchPointCount);
  for (auto idx = 0; idx < touchPointCount; idx++) {
    result.emplace_back(TouchPoint{
        .id = OH_ArkUI_PointerEvent_GetPointerId(event, idx),
        .nodeX = int32_t(OH_ArkUI_PointerEvent_GetXByIndex(event, idx)),
        .nodeY = int32_t(OH_ArkUI_PointerEvent_GetYByIndex(event, idx)),
        .screenX =
            int32_t(OH_ArkUI_PointerEvent_GetDisplayXByIndex(event, idx)),
        .screenY =
            int32_t(OH_ArkUI_PointerEvent_GetDisplayYByIndex(event, idx))});
  }
#endif
  return result;
}

void TouchEventDispatcher::dispatchTouchEvent(
    ArkUI_UIInputEvent* event,
    TouchTarget::Shared const& rootTarget) {
#ifdef C_API_ARCH
  auto action = OH_ArkUI_UIInputEvent_GetAction(event);
  auto timestamp = OH_ArkUI_UIInputEvent_GetEventTime(event);
  auto activeTouch = getActiveTouchFromEvent(event);
  auto touchPoints = getTouchesFromEvent(event);

  VLOG(2) << "Touch event received: id=" << activeTouch.id
          << ", action type:" << action;
  // react-native expects a timestamp in seconds (because rn multiplies the
  // value by 1e3). The timestamp passed by ArkUI is in nanoseconds. We convert
  // it first to miliseconds before casting to lose unnecessary precision. Then
  // we cast it to a double and convert it to seconds.
  double timestampSeconds = static_cast<double>(timestamp) / 1e9;

  if (action == UI_TOUCH_EVENT_ACTION_DOWN) {
    registerTargetForTouch(activeTouch, rootTarget);
  } else if (
      m_touchTargetByTouchId.find(activeTouch.id) !=
      m_touchTargetByTouchId.end()) {
    Point touchPoint{
        .x = static_cast<facebook::react::Float>(activeTouch.nodeX),
        .y = static_cast<facebook::react::Float>(activeTouch.nodeY)};
    auto touchTarget = findTargetForTouchPoint(touchPoint, rootTarget).first;
    auto hasCancelled =
        maybeCancelPreviousTouchEvent(timestampSeconds, touchTarget);
    if (hasCancelled) {
      m_touchTargetByTouchId.erase(activeTouch.id);
      return;
    }
  }

  auto it = m_touchTargetByTouchId.find(activeTouch.id);
  if (it == m_touchTargetByTouchId.end()) {
    VLOG(2) << "No target for current touch event with id: " << activeTouch.id;
    return;
  }
  auto eventTarget = it->second.lock();
  if (eventTarget == nullptr) {
    LOG(WARNING) << "Target for current touch event has been deleted";
    m_touchTargetByTouchId.erase(it);
    return;
  }

  facebook::react::Touches touches;
  std::optional<facebook::react::Touch> changedTouch;
  facebook::react::Touches targetTouches;

  for (auto const& touchPoint : touchPoints) {
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
    if (id == activeTouch.id) {
      changedTouch = touch;
    }
  }

  if (!changedTouch.has_value()) {
    DLOG(INFO) << "No changed touch for current touch event";
    return;
  }

  if (action == UI_TOUCH_EVENT_ACTION_UP) {
    touches.erase(changedTouch.value());
    targetTouches.erase(changedTouch.value());
    m_touchTargetByTouchId.erase(changedTouch.value().identifier);
  }

  facebook::react::TouchEvent touchEvent{
      .touches = std::move(touches),
      .changedTouches = {changedTouch.value()},
      .targetTouches = std::move(targetTouches)};

  if (action == UI_TOUCH_EVENT_ACTION_MOVE && canIgnoreMoveEvent(touchEvent)) {
    VLOG(2) << "Should ignore current touchEvent";
    return;
  }
  m_previousEvent = touchEvent;

  switch (action) {
    case UI_TOUCH_EVENT_ACTION_DOWN:
      eventTarget->getTouchEventEmitter()->onTouchStart(touchEvent);
      break;
    case UI_TOUCH_EVENT_ACTION_MOVE:
      eventTarget->getTouchEventEmitter()->onTouchMove(touchEvent);
      break;
    case UI_TOUCH_EVENT_ACTION_UP:
      eventTarget->getTouchEventEmitter()->onTouchEnd(touchEvent);
      break;
    case UI_TOUCH_EVENT_ACTION_CANCEL:
    default:
      eventTarget->getTouchEventEmitter()->onTouchCancel(touchEvent);
      break;
  }
#endif
}

TouchTarget::Shared TouchEventDispatcher::registerTargetForTouch(
    TouchPoint activeTouch,
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
