#include "SamplePackage.h"
#include "NativeCxxModuleExampleCxxSpec.h"
#include "PropsDisplayerComponentDescriptor.h"
#include "RNOHCorePackage/ComponentBinders/ViewComponentJSIBinder.h"
#include "RNOHCorePackage/ComponentBinders/ViewComponentNapiBinder.h"
#include "SampleTurboModuleSpec.h"
#include "SampleViewComponentDescriptor.h"

using namespace rnoh;
using namespace facebook;

class SampleViewJSIBinder : public ViewComponentJSIBinder {
 protected:
  facebook::jsi::Object createNativeProps(facebook::jsi::Runtime& rt) override {
    auto nativeProps = ViewComponentJSIBinder::createNativeProps(rt);
    nativeProps.setProperty(rt, "size", "true");
    return nativeProps;
  }
};

class SampleTurboModuleFactoryDelegate : public TurboModuleFactoryDelegate {
 public:
  SharedTurboModule createTurboModule(Context ctx, const std::string& name)
      const override {
    if (name == "SampleTurboModule") {
      return std::make_shared<NativeSampleTurboModuleSpecJSI>(ctx, name);
    }
    if (name == "NativeCxxModuleExampleCxx") {
      return std::make_shared<NativeCxxModuleExampleCxxSpecJSI>(ctx, name);
    }
    return nullptr;
  };
};

std::unique_ptr<TurboModuleFactoryDelegate>
SamplePackage::createTurboModuleFactoryDelegate() {
  return std::make_unique<SampleTurboModuleFactoryDelegate>();
}

std::vector<react::ComponentDescriptorProvider>
SamplePackage::createComponentDescriptorProviders() {
  return {
      react::concreteComponentDescriptorProvider<
          react::SampleViewComponentDescriptor>(),
      react::concreteComponentDescriptorProvider<
          react::PropsDisplayerComponentDescriptor>(),
  };
}

ComponentNapiBinderByString SamplePackage::createComponentNapiBinderByName() {
  return {
      {"PropsDisplayer", std::make_shared<ViewComponentNapiBinder>()},
  };
};

ComponentJSIBinderByString SamplePackage::createComponentJSIBinderByName() {
  return {
      {"SampleView", std::make_shared<SampleViewJSIBinder>()},
  };
}

class SampleArkTSMessageHandler : public ArkTSMessageHandler {
  void handleArkTSMessage(const Context& ctx) override {
    auto rnInstance = ctx.rnInstance.lock();
    if (rnInstance) {
      rnInstance->postMessageToArkTS(ctx.messageName, ctx.messagePayload);
    }
  };
};

std::vector<ArkTSMessageHandler::Shared>
SamplePackage::createArkTSMessageHandlers() {
  return {std::make_shared<SampleArkTSMessageHandler>()};
};