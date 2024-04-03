#pragma once
#include "RNOHCorePackage/ComponentBinders/ViewComponentJSIBinder.h"

namespace rnoh {
class SwitchComponentJSIBinder : public ViewComponentJSIBinder {
  facebook::jsi::Object createNativeProps(facebook::jsi::Runtime& rt) override {
    auto nativeProps = ViewComponentJSIBinder::createNativeProps(rt);
    nativeProps.setProperty(rt, "thumbTintColor", "Color");
    nativeProps.setProperty(rt, "onTintColor", "Color");
    nativeProps.setProperty(rt, "disabled", "boolean");
    nativeProps.setProperty(rt, "value", "boolean");

    return nativeProps;
  }
  facebook::jsi::Object createDirectEventTypes(
      facebook::jsi::Runtime& rt) override {
    facebook::jsi::Object events =
        ViewComponentJSIBinder::createDirectEventTypes(rt);

    events.setProperty(rt, "topChange", createDirectEvent(rt, "onChange"));

    return events;
  }
};
} // namespace rnoh