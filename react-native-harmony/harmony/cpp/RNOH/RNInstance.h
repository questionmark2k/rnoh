#ifndef native_RNOHInstance_H
#define native_RNOHInstance_H
#include <memory>
#include <utility>
#include <thread>
#include <functional>
#include <napi/native_api.h>
#include <js_native_api.h>
#include <js_native_api_types.h>

#include <cxxreact/Instance.h>
#include <cxxreact/ModuleRegistry.h>
#include <cxxreact/NativeModule.h>
#include <folly/dynamic.h>
#include <react/renderer/scheduler/Scheduler.h>
#include <react/renderer/componentregistry/ComponentDescriptorProviderRegistry.h>
#include <ReactCommon/LongLivedObject.h>

#include "RNOH/MessageQueueThread.h"
#include "RNOH/SchedulerDelegate.h"
#include "RNOH/TaskExecutor.h"
#include "RNOH/EventEmitterRegistry.h"
#include "RNOH/TurboModuleFactory.h"
#include "RNOH/EventEmitRequestHandler.h"

namespace rnoh {
class RNInstance {
  public:
    using MutationsListener = std::function<void(MutationsToNapiConverter, facebook::react::ShadowViewMutationList const &mutations)>;

    RNInstance(napi_env env,
               TurboModuleFactory &&turboModuleFactory,
               std::shared_ptr<TaskExecutor> taskExecutor,
               std::shared_ptr<react::ComponentDescriptorProviderRegistry> componentDescriptorProviderRegistry,
               MutationsToNapiConverter mutationsToNapiConverter,
               std::string appName,
               EventEmitRequestHandlerByString eventEmitRequestHandlerByName)
        : surfaceHandler(appName, 1),
          instance(std::make_shared<facebook::react::Instance>()),
          scheduler(nullptr),
          taskExecutor(taskExecutor),
          eventEmitterRegistry(std::make_shared<EventEmitterRegistry>()),
          m_turboModuleFactory(std::move(turboModuleFactory)),
          m_componentDescriptorProviderRegistry(componentDescriptorProviderRegistry),
          m_mutationsToNapiConverter(mutationsToNapiConverter),
          m_eventEmitRequestHandlerByName(eventEmitRequestHandlerByName) {}

    void registerSurface(
        MutationsListener,
        MountingManager::CommandDispatcher);
    void start();
    void runApplication(float width, float height, std::string &&bundle);
    void callFunction(std::string &&module, std::string &&method, folly::dynamic &&params);
    void emitComponentEvent(napi_env env, react::Tag tag, std::string eventEmitRequestHandlerName, napi_value payload);

  private:
    std::shared_ptr<facebook::react::ContextContainer> contextContainer;
    std::shared_ptr<facebook::react::Instance> instance;
    std::function<void(MutationsToNapiConverter, facebook::react::ShadowViewMutationList const &mutations)> mutationsListener;
    MountingManager::CommandDispatcher commandDispatcher;
    facebook::react::SurfaceHandler surfaceHandler;
    std::unique_ptr<facebook::react::Scheduler> scheduler;
    std::unique_ptr<SchedulerDelegate> schedulerDelegate;
    std::shared_ptr<TaskExecutor> taskExecutor;
    std::shared_ptr<react::ComponentDescriptorProviderRegistry> m_componentDescriptorProviderRegistry;
    EventEmitterRegistry::Shared eventEmitterRegistry;
    TurboModuleFactory m_turboModuleFactory;
    MutationsToNapiConverter m_mutationsToNapiConverter;
    EventEmitRequestHandlerByString m_eventEmitRequestHandlerByName;

    void initialize();
    void initializeScheduler();
};

} // namespace rnoh
#endif // native_RNOHInstance_H
