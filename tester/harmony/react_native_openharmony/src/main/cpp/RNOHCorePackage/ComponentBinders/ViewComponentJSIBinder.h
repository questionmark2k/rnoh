#pragma once
#include "RNOH/BaseComponentJSIBinder.h"
#include "RNOH/UIManagerModule.h"

namespace rnoh {
class ViewComponentJSIBinder : public BaseComponentJSIBinder {
 protected:
  facebook::jsi::Object createBubblingEventTypes(
      facebook::jsi::Runtime& rt) override {
    facebook::jsi::Object events(rt);
    events.setProperty(
        rt, "topTouchStart", createBubblingCapturedEvent(rt, "onTouchStart"));
    events.setProperty(
        rt, "topTouchMove", createBubblingCapturedEvent(rt, "onTouchMove"));
    events.setProperty(
        rt, "topTouchEnd", createBubblingCapturedEvent(rt, "onTouchEnd"));
    events.setProperty(
        rt, "topTouchCancel", createBubblingCapturedEvent(rt, "onTouchCancel"));
    events.setProperty(
        rt, "topFocus", createBubblingCapturedEvent(rt, "onFocus"));
    events.setProperty(
        rt, "topBlur", createBubblingCapturedEvent(rt, "onBlur"));
    return events;
  }
  facebook::jsi::Object createDirectEventTypes(
      facebook::jsi::Runtime& rt) override {
    facebook::jsi::Object events =
        BaseComponentJSIBinder::createDirectEventTypes(rt);
    events.setProperty(rt, "topClick", createDirectEvent(rt, "onClick"));
    return events;
  }
};

} // namespace rnoh