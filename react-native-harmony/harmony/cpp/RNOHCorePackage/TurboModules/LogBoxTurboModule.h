#pragma once

#include <ReactCommon/TurboModule.h>
#include "RNOH/ArkTSTurboModule.h"

namespace rnoh {

class JSI_EXPORT LogBoxTurboModule : public ArkTSTurboModule {
 public:
  LogBoxTurboModule(
      const ArkTSTurboModule::Context ctx,
      const std::string name);
};

} // namespace rnoh