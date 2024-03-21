#pragma once
#include "RNOHCorePackage/ComponentBinders/ViewComponentJSIBinder.h"

namespace rnoh {

class TextInputComponentJSIBinder : public ViewComponentJSIBinder {
    facebook::jsi::Object createNativeProps(facebook::jsi::Runtime &rt) override {
        auto nativeProps = ViewComponentJSIBinder::createNativeProps(rt);
        nativeProps.setProperty(rt, "text", "string");
        nativeProps.setProperty(rt, "editable", "boolean");
        nativeProps.setProperty(rt, "multiline", "boolean");
        nativeProps.setProperty(rt, "placeholder", "string");
        nativeProps.setProperty(rt, "placeholderTextColor", "Color");
        nativeProps.setProperty(rt, "cursorColor", "Color");
        nativeProps.setProperty(rt, "caretHidden", "boolean");
        nativeProps.setProperty(rt, "secureTextEntry", "boolean");
        nativeProps.setProperty(rt, "maxLength", "number");
        nativeProps.setProperty(rt, "selectionColor", "Color");
        nativeProps.setProperty(rt, "returnKeyType", "ReturnKeyType");
        nativeProps.setProperty(rt, "textAlign", "string");
        nativeProps.setProperty(rt, "autoFocus", "boolean");
        nativeProps.setProperty(rt, "keyboardType", "KeyboardType");
        nativeProps.setProperty(rt, "allowFontScaling", "boolean");
        nativeProps.setProperty(rt, "clearTextOnFocus", "boolean");
        nativeProps.setProperty(rt, "mostRecentEventCount", "number");
        nativeProps.setProperty(rt, "clearButtonMode", "string");
        nativeProps.setProperty(rt, "defaultValue", "string");
        nativeProps.setProperty(rt, "selectTextOnFocus", "boolean");
        nativeProps.setProperty(rt, "contextMenuHidden", "boolean");

        return nativeProps;
    };
    facebook::jsi::Object createDirectEventTypes(facebook::jsi::Runtime &rt) override {
        facebook::jsi::Object events = ViewComponentJSIBinder::createDirectEventTypes(rt);

        events.setProperty(rt, "topChange", createDirectEvent(rt, "onChange"));
        events.setProperty(rt, "topSubmitEditing", createDirectEvent(rt, "onSubmitEditing"));
        events.setProperty(rt, "topEndEditing", createDirectEvent(rt, "onEndEditing"));
        events.setProperty(rt, "topKeyPress", createDirectEvent(rt, "onKeyPress"));
        events.setProperty(rt, "topSelectionChange", createDirectEvent(rt, "topSelectionChange"));

        return events;
    }
};

} // namespace rnoh