#pragma once

#include <ReactCommon/TurboModule.h>
#include "RNOH/ArkTSTurboModule.h"

namespace rnoh {

class JSI_EXPORT ShareTurboModule : public ArkTSTurboModule {
 public:
  ShareTurboModule(const ArkTSTurboModule::Context ctx, const std::string name);
};

} // namespace rnoh
