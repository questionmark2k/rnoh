import mustache from 'mustache';

const TEMPLATE = `
#pragma once

// This file was generated.

#include <react/renderer/core/ConcreteComponentDescriptor.h>
#include <react/renderer/components/view/ConcreteViewShadowNode.h>
#include <react/renderer/components/view/ViewShadowNode.h>

namespace facebook {
namespace react {

extern const char {{name}}ComponentName[] = "{{name}}";

class {{name}}Props : public ViewProps {
  public:
    {{name}}Props() = default;

    {{name}}Props(const PropsParserContext &context, const {{name}}Props &sourceProps, const RawProps &rawProps)
        : ViewProps(context, sourceProps, rawProps) {}
};

using {{name}}ShadowNode = ConcreteViewShadowNode<
    {{name}}ComponentName,
    {{name}}Props,
    ViewEventEmitter>;

class {{name}}ComponentDescriptor final
    : public ConcreteComponentDescriptor<{{name}}ShadowNode> {
  public:
    {{name}}ComponentDescriptor(ComponentDescriptorParameters const &parameters)
        : ConcreteComponentDescriptor(parameters) {}
};

} // namespace react
} // namespace facebook
`;

export class ComponentDescriptorHTemplate {
  constructor(private name: string) {}

  build(): string {
    return mustache.render(TEMPLATE, {
      name: this.name,
    });
  }
}
