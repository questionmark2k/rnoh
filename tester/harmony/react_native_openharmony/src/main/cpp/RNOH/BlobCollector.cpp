#include "RNOH/BlobCollector.h"
#include <jsi/jsi.h>
#include "ArkTSTurboModule.h"
#include "BlobCollector.h"
#include "RNOH/TurboModule.h"
#include "RNOHCorePackage/TurboModules/BlobTurboModule.h"

namespace rnoh {

/**
 * This is a hack to get around the fact that there's no easy way to call a
 * method in a TurboModule at the destruction of an object. Because we add the
 * BlobCollector as a HostObject to the Blob in js, it's destructor will be
 * called when the Blob is garbage collected.
 */
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