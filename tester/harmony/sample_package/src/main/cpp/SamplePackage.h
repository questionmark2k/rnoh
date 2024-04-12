#include "RNOH/Package.h"

namespace rnoh {

class SamplePackage : public Package {
 public:
  SamplePackage(Package::Context ctx) : Package(ctx) {}

  std::vector<facebook::react::ComponentDescriptorProvider>
  createComponentDescriptorProviders() override;

  std::unique_ptr<TurboModuleFactoryDelegate> createTurboModuleFactoryDelegate()
      override;

  ComponentNapiBinderByString createComponentNapiBinderByName() override;

  ComponentJSIBinderByString createComponentJSIBinderByName() override;

  std::vector<ArkTSMessageHandler::Shared> createArkTSMessageHandlers()
      override;

  ComponentInstanceFactoryDelegate::Shared
  createComponentInstanceFactoryDelegate() override;
};
} // namespace rnoh
