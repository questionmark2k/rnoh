#include "RNOHCorePackage/TurboModules/AccessibilityInfoTurboModule.h"

namespace rnoh {
using namespace facebook;

AccessibilityInfoTurboModule::AccessibilityInfoTurboModule(
    const ArkTSTurboModule::Context ctx,
    const std::string name)
    : ArkTSTurboModule(ctx, name) {
  methodMap_ = {ARK_METHOD_METADATA(isTouchExplorationEnabled, 1)};
}

} // namespace rnoh
