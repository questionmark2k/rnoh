#pragma once
#include <react/renderer/componentregistry/ComponentDescriptorProvider.h>
#include "RNOH/GlobalJSIBinder.h"
#include "RNOH/TurboModule.h"
#include "RNOH/ShadowViewRegistry.h"
#include "RNOH/TurboModuleFactory.h"
#include "RNOH/UIManagerModule.h"
#include "RNOH/MutationsToNapiConverter.h"
#include "RNOH/EventEmitRequestHandler.h"
#include "RNOH/ComponentInstanceFactory.h"

namespace rnoh {

class Package {
  public:
    struct Context : ComponentInstance::Context {
      ShadowViewRegistry::Shared shadowViewRegistry;
    };

    Package(Context ctx);
    virtual ~Package(){};

    virtual std::unique_ptr<TurboModuleFactoryDelegate> createTurboModuleFactoryDelegate();

    virtual GlobalJSIBinders createGlobalJSIBinders();

    virtual std::vector<facebook::react::ComponentDescriptorProvider> createComponentDescriptorProviders();

    virtual ComponentJSIBinderByString createComponentJSIBinderByName();

    virtual ComponentNapiBinderByString createComponentNapiBinderByName();

    virtual EventEmitRequestHandlers createEventEmitRequestHandlers();

    /**
     * Used only in C-API based Architecture.
     */
    virtual ComponentInstanceFactoryDelegate::Shared createComponentInstanceFactoryDelegate();

  protected:
    Context m_ctx;
};
} // namespace rnoh