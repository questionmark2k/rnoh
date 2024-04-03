#include "RNOHCorePackage/TurboModules/LogBoxTurboModule.h"

using namespace facebook;

namespace rnoh {
LogBoxTurboModule::LogBoxTurboModule(
    const ArkTSTurboModule::Context ctx,
    const std::string name)
    : ArkTSTurboModule(ctx, name) {
  methodMap_ = {
      ARK_METHOD_METADATA(show, 0),
      ARK_METHOD_METADATA(hide, 0),
  };
}
} // namespace rnoh
