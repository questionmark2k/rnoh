#include "RNOHCorePackage/TurboModules/ShareTurboModule.h"

using namespace facebook;

namespace rnoh {
ShareTurboModule::ShareTurboModule(
    const ArkTSTurboModule::Context ctx,
    const std::string name)
    : ArkTSTurboModule(ctx, name) {
  methodMap_ = {
      ARK_ASYNC_METHOD_METADATA(share, 2),
  };
}
} // namespace rnoh
