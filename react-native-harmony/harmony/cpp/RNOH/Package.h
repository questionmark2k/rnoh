#pragma once
#include <react/renderer/componentregistry/ComponentDescriptorProvider.h>
#include "RNOH/ArkTSMessageHandler.h"
#include "RNOH/ComponentInstanceFactory.h"
#include "RNOH/EventEmitRequestHandler.h"
#include "RNOH/GlobalJSIBinder.h"
#include "RNOH/MutationsToNapiConverter.h"
#include "RNOH/ShadowViewRegistry.h"
#include "RNOH/TurboModule.h"
#include "RNOH/TurboModuleFactory.h"
#include "RNOH/UIManagerModule.h"

namespace rnoh {

class Package {
 public:
  struct Context : ComponentInstance::Context {
    ShadowViewRegistry::Shared shadowViewRegistry;
  };

  Package(Context ctx);
  virtual ~Package(){};

  virtual std::unique_ptr<TurboModuleFactoryDelegate>
  createTurboModuleFactoryDelegate();

  virtual GlobalJSIBinders createGlobalJSIBinders();

  virtual std::vector<facebook::react::ComponentDescriptorProvider>
  createComponentDescriptorProviders();

  virtual ComponentJSIBinderByString createComponentJSIBinderByName();

  virtual ComponentNapiBinderByString createComponentNapiBinderByName();

  virtual EventEmitRequestHandlers createEventEmitRequestHandlers();

  virtual std::vector<ArkTSMessageHandler::Shared>
  createArkTSMessageHandlers() {
    return {};
  };

  /**
   * Used only in C-API based Architecture.
   */
  virtual ComponentInstanceFactoryDelegate::Shared
  createComponentInstanceFactoryDelegate();

 protected:
  Context m_ctx;
};
} // namespace rnoh