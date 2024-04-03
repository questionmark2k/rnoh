#include "RNOH/BlobCollector.h"
#include <jsi/jsi.h>
#include "ArkTSTurboModule.h"
#include "BlobCollector.h"
#include "RNOH/TurboModule.h"
#include "RNOHCorePackage/TurboModules/BlobTurboModule.h"

namespace rnoh {

BlobCollector::BlobCollector(
    facebook::react::TurboModule& turboModule,
    const std::string& blobId)
    : turboModule_(turboModule), blobId_(blobId){};

void BlobCollector::install(
    facebook::jsi::Runtime& rt,
    facebook::react::TurboModule& turboModule) {
  rt.global().setProperty(
      rt,
      "__blobCollectorProvider",
      facebook::jsi::Function::createFromHostFunction(
          rt,
          facebook::jsi::PropNameID::forAscii(rt, "__blobCollectorProvider"),
          1,
          [&turboModule](
              facebook::jsi::Runtime& rt,
              const facebook::jsi::Value& thisVal,
              const facebook::jsi::Value* args,
              size_t count) {
            auto blobId = args[0].asString(rt).utf8(rt);
            auto blobCollector =
                std::make_shared<BlobCollector>(turboModule, blobId);
            return facebook::jsi::Object::createFromHostObject(
                rt, blobCollector);
          }));
}

BlobCollector::~BlobCollector() {
  static_cast<BlobTurboModule&>(turboModule_).release(blobId_);
}

} // namespace rnoh