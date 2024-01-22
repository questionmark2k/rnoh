#include "RNOHCorePackage/TurboModules/BlobTurboModule.h"
#include "RNOH/BlobCollector.h"


namespace rnoh {
    using namespace facebook;

    BlobTurboModule::BlobTurboModule(const ArkTSTurboModule::Context ctx, const std::string name)
        : ArkTSTurboModule(ctx, name) {
        methodMap_ = {
            ARK_METHOD_METADATA(getConstants, 0),
            ARK_METHOD_METADATA(release, 1),
            ARK_METHOD_METADATA(sendOverSocket, 2),
            ARK_METHOD_METADATA(addWebSocketHandler, 1),
            ARK_METHOD_METADATA(addNetworkingHandler, 0),
            ARK_METHOD_METADATA(removeWebSocketHandler, 1),
            // this is a hack needed to install blobCollector,
            {"createFromParts",
             {2, [](facebook::jsi::Runtime &rt, facebook::react::TurboModule &turboModule,
                    const facebook::jsi::Value *args, size_t count) {
                  BlobCollector::install(rt, turboModule, args, count);
                  return static_cast<ArkTSTurboModule &>(turboModule).call(rt, "createFromParts", args, count);
              }}}};
    }

    void BlobTurboModule::release(std::string blobId) {
        m_ctx.taskExecutor->runTask(TaskThread::MAIN, [ctx = m_ctx, name = name_, blobId]() {
            std::string methodName = "release";
            try {
                ArkJS arkJs(ctx.env);
                std::vector<napi_value> napiArgs;
                napiArgs.push_back(arkJs.createString(blobId));
                auto napiTurboModuleObject = arkJs.getObject(ctx.arkTsTurboModuleInstanceRef);
                napiTurboModuleObject.call(methodName, napiArgs);
            } catch (const std::exception &e) {
                LOG(ERROR) << "Exception thrown while calling " << name << " TurboModule method " << methodName << ": "
                           << e.what();
            }
        });
    }

} // namespace rnoh