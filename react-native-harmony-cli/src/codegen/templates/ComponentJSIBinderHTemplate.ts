import mustache from 'mustache';

const TEMPLATE = `
#pragma once

// This file was generated.

#include "RNOHCorePackage/ComponentBinders/ViewComponentJSIBinder.h"

namespace rnoh {
class {{name}}JSIBinder : public ViewComponentJSIBinder {
  protected:
    facebook::jsi::Object createNativeProps(facebook::jsi::Runtime &rt) override {
        auto object = ViewComponentJSIBinder::createNativeProps(rt);
        {{#props}}
        object.setProperty(rt, "{{name}}", {{{type}}});
        {{/props}}
        return object;
    }

    facebook::jsi::Object createBubblingEventTypes(facebook::jsi::Runtime &rt) override {
        facebook::jsi::Object events(rt);
        {{#bubblingEvents}}
        events.setProperty(rt, "{{TOPname}}", createBubblingCapturedEvent(rt, "{{name}}"));
        {{/bubblingEvents}}
        return events;
    }

    facebook::jsi::Object createDirectEventTypes(facebook::jsi::Runtime &rt) override {
        facebook::jsi::Object events(rt);
        {{#directEvents}}
        events.setProperty(rt, "{{TOPname}}", createDirectEvent(rt, "{{name}}"));
        {{/directEvents}}
        return events;
    }
};
} // namespace rnoh
`;

type ComponentJSIBinderHProp = {
  name: string;
  isObject: boolean;
};

type ComponentJSIBinderHEvent = {
  type: 'direct' | 'bubble';
  eventHandlerName: string;
};

export class ComponentJSIBinderHTemplate {
  private props: ComponentJSIBinderHProp[] = [];
  private events: ComponentJSIBinderHEvent[] = [];

  constructor(private name: string) {}

  addProp(prop: ComponentJSIBinderHProp) {
    this.props.push(prop);
  }

  addEvent(event: ComponentJSIBinderHEvent) {
    this.events.push(event);
  }

  build(): string {
    return mustache.render(TEMPLATE.trimStart(), {
      name: this.name,
      props: this.props.map((prop) => ({
        name: prop.name,
        type: prop.isObject ? '"Object"' : 'true',
      })),
      bubblingEvents: this.events
        .filter((event) => event.type === 'bubble')
        .map((event) => ({
          TOPname: event.eventHandlerName.replace('on', 'top'),
          name: event.eventHandlerName,
        })),
      directEvents: this.events
        .filter((event) => event.type === 'direct')
        .map((event) => ({
          TOPname: event.eventHandlerName.replace('on', 'top'),
          name: event.eventHandlerName,
        })),
    });
  }
}
