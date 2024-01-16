import mustache from 'mustache';

const TEMPLATE = `
#pragma once

#include "RNOH/ArkTSTurboModule.h"

namespace rnoh {

class JSI_EXPORT {{className}} : public ArkTSTurboModule {
  public:
    {{className}}(const ArkTSTurboModule::Context ctx, const std::string name);
};

} // namespace rnoh
`;

export class TurboModuleHTemplate {
  constructor(private name: string) {}

  build(): string {
    return mustache.render(TEMPLATE, { className: this.name });
  }
}
