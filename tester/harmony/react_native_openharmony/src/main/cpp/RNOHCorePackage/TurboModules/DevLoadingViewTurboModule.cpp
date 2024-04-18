#include "RNOHCorePackage/TurboModules/DevLoadingViewTurboModule.h"

using namespace facebook;

namespace rnoh {
DevLoadingViewTurboModule::DevLoadingViewTurboModule(
    const ArkTSTurboModule::Context ctx,
    const std::string name)
    : ArkTSTurboModule(ctx, name) {
  methodMap_ = {
      ARK_METHOD_METADATA(showMessage, 3),
      ARK_METHOD_METADATA(hide, 0),
  };
}
} // namespace rnoh
