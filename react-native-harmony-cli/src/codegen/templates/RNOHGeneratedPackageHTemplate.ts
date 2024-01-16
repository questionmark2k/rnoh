import mustache from 'mustache';

const TEMPLATE = `
#pragma once
#include "RNOH/Package.h"
#include "RNOH/ArkTSTurboModule.h"
{{#turboModules}}
#include "generated/{{name}}.h"
{{/turboModules}}

namespace rnoh {

class RNOHGeneratedPackageTurboModuleFactoryDelegate : public TurboModuleFactoryDelegate {
  public:
    SharedTurboModule createTurboModule(Context ctx, const std::string &name) const override {
        {{#turboModules}}
        if (name == "{{name}}") {
            return std::make_shared<{{name}}>(ctx, name);
        }
        {{/turboModules}}
        return nullptr;
    };
};

class RNOHGeneratedPackage : public Package {
  public:
    RNOHGeneratedPackage(Package::Context ctx) : Package(ctx){};

    std::unique_ptr<TurboModuleFactoryDelegate> createTurboModuleFactoryDelegate() override {
        return std::make_unique<RNOHGeneratedPackageTurboModuleFactoryDelegate>();
    }
};

} // namespace rnoh
`;

type RNOHGeneratedPackageTurboModule = {
  name: string;
};

export class RNOHGeneratedPackageHTemplate {
  private turboModules: RNOHGeneratedPackageTurboModule[] = [];

  constructor() {}

  addTurboModule(turboModule: RNOHGeneratedPackageTurboModule) {
    this.turboModules.push(turboModule);
  }

  build(): string {
    return mustache.render(TEMPLATE, { turboModules: this.turboModules });
  }
}
