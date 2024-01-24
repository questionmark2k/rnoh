import mustache from 'mustache';

const TEMPLATE = `
#include "{{className}}.h"

// This file was generated.

namespace rnoh {
using namespace facebook;

{{className}}::{{className}}(const ArkTSTurboModule::Context ctx, const std::string name) : ArkTSTurboModule(ctx, name) {
    methodMap_ = {
        {{#methods}}
        ARK_{{asyncMarker}}METHOD_METADATA({{name}}, {{argsCount}}),
        {{/methods}}
    };
}

} // namespace rnoh
`;

export type TurboModuleCppTemplateMethod = {
  name: string;
  isAsync: boolean;
  argsCount: number;
};

export class TurboModuleCppTemplate {
  private methods: TurboModuleCppTemplateMethod[] = [];

  constructor(private name: string) {}

  addMethod(method: TurboModuleCppTemplateMethod) {
    this.methods.push(method);
  }

  build(): string {
    return mustache.render(TEMPLATE, {
      className: this.name,
      methods: this.methods.map((method) => ({
        name: method.name,
        argsCount: method.argsCount,
        asyncMarker: method.isAsync ? 'ASYNC_' : undefined,
      })),
    });
  }
}
