#include "RNOHCorePackage/TurboModules/BlobTurboModule.h"

namespace rnoh {
using namespace facebook;

BlobTurboModule::BlobTurboModule(const ArkTSTurboModule::Context ctx, const std::string name) : ArkTSTurboModule(ctx, name) {
    methodMap_ = {
        ARK_METHOD_METADATA(getConstants, 0),
        ARK_METHOD_METADATA(createFromParts, 2),
        ARK_METHOD_METADATA(release, 1),
        ARK_METHOD_METADATA(sendOverSocket, 2),
        ARK_METHOD_METADATA(addWebSocketHandler, 1),
        ARK_METHOD_METADATA(removeWebSocketHandler, 1)
    };
}

} // namespace rnoh