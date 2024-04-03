#include "RNOHCorePackage/TurboModules/FileReaderTurboModule.h"

namespace rnoh {
using namespace facebook;

FileReaderTurboModule::FileReaderTurboModule(
    const ArkTSTurboModule::Context ctx,
    const std::string name)
    : ArkTSTurboModule(ctx, name) {
  methodMap_ = {
      ARK_ASYNC_METHOD_METADATA(readAsDataURL, 1),
      ARK_ASYNC_METHOD_METADATA(readAsText, 2)};
}

} // namespace rnoh