#pragma once

#include <ReactCommon/TurboModule.h>
#include "RNOH/ArkTSTurboModule.h"

namespace rnoh {

class JSI_EXPORT BlobTurboModule : public ArkTSTurboModule {
 public:
  BlobTurboModule(const ArkTSTurboModule::Context ctx, const std::string name);
  void release(const std::string blobId);
};

} // namespace rnoh