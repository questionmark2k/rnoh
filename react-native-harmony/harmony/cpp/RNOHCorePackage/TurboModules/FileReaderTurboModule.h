#pragma once

#include <ReactCommon/TurboModule.h>
#include "RNOH/ArkTSTurboModule.h"

namespace rnoh {

class JSI_EXPORT FileReaderTurboModule : public ArkTSTurboModule {
 public:
  FileReaderTurboModule(
      const ArkTSTurboModule::Context ctx,
      const std::string name);
};

} // namespace rnoh