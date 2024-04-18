#include "RNOHCorePackage/TurboModules/PlatformColorTurboModule.h"

namespace rnoh {
using namespace facebook;

PlatformColorTurboModule::PlatformColorTurboModule(
    const ArkTSTurboModule::Context ctx,
    const std::string name)
    : ArkTSTurboModule(ctx, name) {
  methodMap_ = {ARK_METHOD_METADATA(getSystemColor, 1)};
}

} // namespace rnoh
