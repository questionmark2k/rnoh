import mustache from 'mustache';

const TEMPLATE = `
#pragma once

// This file was generated.

#include "RNOH/Package.h"
#include "RNOH/ArkTSTurboModule.h"
{{#turboModules}}
#include "generated/{{name}}.h"
{{/turboModules}}
{{#components}}
#include "generated/{{name}}ComponentDescriptor.h"
{{/components}}
{{#components}}
#include "generated/{{name}}JSIBinder.h"
{{/components}}

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

class GeneratedEventEmitRequestHandler : public EventEmitRequestHandler {
  public:
    void handleEvent(Context const &ctx) override {
        auto eventEmitter = ctx.shadowViewRegistry->getEventEmitter<facebook::react::EventEmitter>(ctx.tag);
        if (eventEmitter == nullptr) {
            return;
        }

        std::vector<std::string> supportedEventNames = {
            {{#supportedEvents}}
            "{{name}}",
            {{/supportedEvents}}
        };
        if (std::find(supportedEventNames.begin(), supportedEventNames.end(), ctx.eventName) != supportedEventNames.end()) {
            eventEmitter->dispatchEvent(ctx.eventName, ArkJS(ctx.env).getDynamic(ctx.payload));
        }    
    }
};

class RNOHGeneratedPackage : public Package {
  public:
    RNOHGeneratedPackage(Package::Context ctx) : Package(ctx){};

    std::unique_ptr<TurboModuleFactoryDelegate> createTurboModuleFactoryDelegate() override {
        return std::make_unique<RNOHGeneratedPackageTurboModuleFactoryDelegate>();
    }

    std::vector<facebook::react::ComponentDescriptorProvider> createComponentDescriptorProviders() override {
        return {
            {{#components}}
            facebook::react::concreteComponentDescriptorProvider<facebook::react::{{name}}ComponentDescriptor>(),
            {{/components}}
        };
    }

    ComponentJSIBinderByString createComponentJSIBinderByName() override {
        return {
            {{#components}}
            {"{{name}}", std::make_shared<{{name}}JSIBinder>()},
            {{/components}}
        };
    };

    EventEmitRequestHandlers createEventEmitRequestHandlers() override {
        return {
            std::make_shared<GeneratedEventEmitRequestHandler>(),
        };
    }
};

} // namespace rnoh
`;

type RNOHGeneratedPackageTurboModule = {
  name: string;
};

type RNOHGeneratedPackageComponent = {
  name: string;
  supportedEventNames: string[];
};

export class RNOHGeneratedPackageHTemplate {
  private turboModules: RNOHGeneratedPackageTurboModule[] = [];
  private components: RNOHGeneratedPackageComponent[] = [];
  private supportedEventNames: string[] = [];

  constructor() {}

  addTurboModule(turboModule: RNOHGeneratedPackageTurboModule) {
    this.turboModules.push(turboModule);
  }

  addComponent(component: RNOHGeneratedPackageComponent) {
    this.components.push(component);
    this.supportedEventNames = [
      ...this.supportedEventNames,
      ...component.supportedEventNames,
    ];
  }

  build(): string {
    return mustache.render(TEMPLATE.trimStart(), {
      turboModules: this.turboModules,
      components: this.components,
      supportedEvents: this.supportedEventNames.map((eventName) => ({
        name: eventName,
      })),
    });
  }
}
