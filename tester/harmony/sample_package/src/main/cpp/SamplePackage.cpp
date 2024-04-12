#include "SamplePackage.h"
#include "NativeCxxModuleExampleCxxSpec.h"
#include "PropsDisplayerComponentDescriptor.h"
#include "RNOH/RNInstanceCAPI.h"
#include "RNOHCorePackage/ComponentBinders/ViewComponentJSIBinder.h"
#include "RNOHCorePackage/ComponentBinders/ViewComponentNapiBinder.h"
#include "SampleTurboModuleSpec.h"
#include "SampleViewComponentDescriptor.h"
#include "SampleViewComponentInstance.h"

using namespace rnoh;
using namespace facebook;

class SampleViewJSIBinder : public ViewComponentJSIBinder {
 protected:
  facebook::jsi::Object createNativeProps(facebook::jsi::Runtime& rt) override {
    auto nativeProps = ViewComponentJSIBinder::createNativeProps(rt);
    nativeProps.setProperty(rt, "size", "true");
    nativeProps.setProperty(rt, "textColor", "true");
    return nativeProps;
  }

  facebook::jsi::Object createDirectEventTypes(
      facebook::jsi::Runtime& rt) override {
    facebook::jsi::Object events =
        ViewComponentJSIBinder::createDirectEventTypes(rt);
    events.setProperty(
        rt, "topSampleClick", createDirectEvent(rt, "onSampleClick"));
    return events;
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
      if (ctx.messageName == "SAMPLE_MESSAGE") {
        rnInstance->postMessageToArkTS(ctx.messageName, ctx.messagePayload);
      } else if (ctx.messageName == "BLOCK_NATIVE_RESPONDER") {
        auto rnInstanceCAPI =
            std::dynamic_pointer_cast<RNInstanceCAPI>(rnInstance);
        auto maybeTag = rnInstanceCAPI->findComponentInstanceTagById(
            ctx.messagePayload["componentInstanceId"].asString());
        if (maybeTag.has_value()) {
          auto componentInstance =
              rnInstanceCAPI->findComponentInstanceByTag(maybeTag.value());
          componentInstance->setNativeResponderBlocked(
              ctx.messagePayload["isBlocked"].asBool(),
              ctx.messagePayload["origin"].asString());
        }
      }
    }
  };
};

std::vector<ArkTSMessageHandler::Shared>
SamplePackage::createArkTSMessageHandlers() {
  return {std::make_shared<SampleArkTSMessageHandler>()};
}

ComponentInstanceFactoryDelegate::Shared
rnoh::SamplePackage::createComponentInstanceFactoryDelegate() {
  class SampleComponentInstanceFactoryDelegate
      : public ComponentInstanceFactoryDelegate {
   public:
    ComponentInstance::Shared create(ComponentInstance::Context ctx) override {
      if (ctx.componentName == "SampleView") {
        return std::make_shared<SampleViewComponentInstance>(ctx);
      }
      return nullptr;
    }
  };
  return std::make_shared<SampleComponentInstanceFactoryDelegate>();
};