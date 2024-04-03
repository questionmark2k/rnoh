#pragma once
#include <jsi/jsi.h>
#include <string>
#include "ReactCommon/TurboModule.h"

namespace rnoh {

class BlobCollector : public facebook::jsi::HostObject {
 public:
  BlobCollector(
      facebook::react::TurboModule& turboModule,
      const std::string& blobId);
  ~BlobCollector();

  static void install(
      facebook::jsi::Runtime& rt,
      facebook::react::TurboModule& turboModule);

 private:
  const std::string blobId_;
  facebook::react::TurboModule& turboModule_;
};

} // namespace rnoh