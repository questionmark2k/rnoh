#include "RNOHCorePackage/TurboModules/VibrationTurboModule.h"

using namespace facebook;

namespace rnoh {
VibrationTurboModule::VibrationTurboModule(
    const ArkTSTurboModule::Context ctx,
    const std::string name)
    : ArkTSTurboModule(ctx, name) {
  methodMap_ = {
      ARK_METHOD_METADATA(vibrate, 1),
      ARK_METHOD_METADATA(cancel, 0),
  };
}
} // namespace rnoh
