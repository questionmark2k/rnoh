#pragma once

#include <arkui/native_event.h>
#include <arkui/ui_input_event.h>
#include <react/renderer/graphics/Point.h>
#include <unordered_map>
#include "RNOH/TouchTarget.h"

namespace rnoh {
struct TouchPoint {
  int32_t id;
  int32_t nodeX;
  int32_t nodeY;
  int32_t screenX;
  int32_t screenY;
};
class TouchEventDispatcher {
 public:
  using TouchId = int;

  void dispatchTouchEvent(
      ArkUI_UIInputEvent* event,
      TouchTarget::Shared const& rootTarget);

 private:
  TouchTarget::Shared registerTargetForTouch(
      TouchPoint touchPoint,
      TouchTarget::Shared const& rootTarget);
  bool canIgnoreMoveEvent(facebook::react::TouchEvent currentEvent);
  bool maybeCancelPreviousTouchEvent(double timestampInMs, TouchTarget::Shared);
  void shouldCancelTouchesForTag(facebook::react::Tag tag);

  std::unordered_map<TouchId, TouchTarget::Weak> m_touchTargetByTouchId;
  facebook::react::TouchEvent m_previousEvent;
};
} // namespace rnoh