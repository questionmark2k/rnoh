#pragma once

#include <ReactCommon/TurboModule.h>
#include "RNOH/ArkTSTurboModule.h"

namespace rnoh {

class JSI_EXPORT VibrationTurboModule : public ArkTSTurboModule {
 public:
  VibrationTurboModule(
      const ArkTSTurboModule::Context ctx,
      const std::string name);
};

} // namespace rnoh
