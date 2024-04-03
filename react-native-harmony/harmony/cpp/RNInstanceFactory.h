#pragma once
#include <react/renderer/components/image/ImageComponentDescriptor.h>
#include <react/renderer/components/text/ParagraphComponentDescriptor.h>
#include <react/renderer/mounting/ShadowViewMutation.h>
#include <array>
#include <memory>
#include <string>
#include <vector>
#include "RNOH/ArkJS.h"
#include "RNOH/ArkTSChannel.h"
#include "RNOH/ArkTSMessageHandler.h"
#include "RNOH/ArkTSTurboModule.h"
#include "RNOH/EventEmitRequestHandler.h"
#include "RNOH/FeatureFlagRegistry.h"
#include "RNOH/MutationsToNapiConverter.h"
#include "RNOH/PackageProvider.h"
#include "RNOH/RNInstance.h"
#include "RNOH/RNInstanceArkTS.h"
#include "RNOH/RNInstanceCAPI.h"
#include "RNOH/SchedulerDelegateArkTS.h"
#include "RNOH/TextMeasurer.h"
#include "RNOH/TurboModuleFactory.h"
#include "RNOH/UITicker.h"
#include "RNOHCorePackage/RNOHCorePackage.h"

#ifdef C_API_ARCH
#include "RNOH/ComponentInstanceFactory.h"
#include "RNOH/ComponentInstanceRegistry.h"
#include "RNOH/CustomComponentArkUINodeHandleFactory.h"
#include "RNOH/SchedulerDelegateCAPI.h"
#endif

using namespace rnoh;

std::shared_ptr<RNInstanceInternal> createRNInstance(
    int id,
    napi_env env,
    napi_ref arkTsTurboModuleProviderRef,
    napi_ref frameNodeFactoryRef,
    MutationsListener&& mutationsListener,
    MountingManager::CommandDispatcher&& commandDispatcher,
    napi_ref measureTextFnRef,
    napi_ref napiEventDispatcherRef,
    FeatureFlagRegistry::Shared featureFlagRegistry,
    UITicker::Shared uiTicker,
    bool shouldEnableDebugger,
    bool shouldEnableBackgroundExecutor) {
  auto shouldUseCAPIArchitecture =
      featureFlagRegistry->getFeatureFlagStatus("C_API_ARCH");
  std::shared_ptr<TaskExecutor> taskExecutor =
      std::make_shared<TaskExecutor>(env, shouldEnableBackgroundExecutor);
  auto arkTSChannel = std::make_shared<ArkTSChannel>(
      taskExecutor, ArkJS(env), napiEventDispatcherRef);

  taskExecutor->setExceptionHandler(
      [weakExecutor = std::weak_ptr(taskExecutor)](std::exception_ptr e) {
        auto executor = weakExecutor.lock();
        if (executor == nullptr) {
          return;
        }
        executor->runTask(TaskThread::MAIN, [e]() {
          ArkTSBridge::getInstance()->handleError(e);
        });
      });

  auto contextContainer = std::make_shared<facebook::react::ContextContainer>();
  auto textMeasurer = std::make_shared<TextMeasurer>(
      env, measureTextFnRef, taskExecutor, featureFlagRegistry);
  auto shadowViewRegistry = std::make_shared<ShadowViewRegistry>();
  contextContainer->insert("textLayoutManagerDelegate", textMeasurer);
  PackageProvider packageProvider;
  auto packages = packageProvider.getPackages({});
  packages.insert(
      packages.begin(),
      std::make_shared<RNOHCorePackage>(
          Package::Context{.shadowViewRegistry = shadowViewRegistry}));

  auto componentDescriptorProviderRegistry =
      std::make_shared<facebook::react::ComponentDescriptorProviderRegistry>();
  std::vector<std::shared_ptr<TurboModuleFactoryDelegate>>
      turboModuleFactoryDelegates;
  ComponentJSIBinderByString componentJSIBinderByName = {};
  GlobalJSIBinders globalJSIBinders = {};
  ComponentNapiBinderByString componentNapiBinderByName = {};
  EventEmitRequestHandlers eventEmitRequestHandlers = {};
  std::vector<ComponentInstanceFactoryDelegate::Shared>
      componentInstanceFactoryDelegates = {};
  std::vector<ArkTSMessageHandler::Shared> arkTSMessageHandlers = {};

  for (auto& package : packages) {
    auto turboModuleFactoryDelegate =
        package->createTurboModuleFactoryDelegate();
    if (turboModuleFactoryDelegate != nullptr) {
      turboModuleFactoryDelegates.push_back(
          std::move(turboModuleFactoryDelegate));
    }
    for (auto componentDescriptorProvider :
         package->createComponentDescriptorProviders()) {
      componentDescriptorProviderRegistry->add(componentDescriptorProvider);
    }
    for (auto [name, componentJSIBinder] :
         package->createComponentJSIBinderByName()) {
      componentJSIBinderByName.insert({name, componentJSIBinder});
    };
    for (auto [name, componentNapiBinder] :
         package->createComponentNapiBinderByName()) {
      componentNapiBinderByName.insert({name, componentNapiBinder});
    };
    auto packageGlobalJSIBinders = package->createGlobalJSIBinders();
    globalJSIBinders.insert(
        globalJSIBinders.end(),
        std::make_move_iterator(packageGlobalJSIBinders.begin()),
        std::make_move_iterator(packageGlobalJSIBinders.end()));
    auto packageEventEmitRequestHandlers =
        package->createEventEmitRequestHandlers();
    eventEmitRequestHandlers.insert(
        eventEmitRequestHandlers.end(),
        std::make_move_iterator(packageEventEmitRequestHandlers.begin()),
        std::make_move_iterator(packageEventEmitRequestHandlers.end()));
    auto componentInstanceFactoryDelegate =
        package->createComponentInstanceFactoryDelegate();
    if (componentInstanceFactoryDelegate != nullptr) {
      componentInstanceFactoryDelegates.push_back(
          std::move(componentInstanceFactoryDelegate));
    }
    for (auto const& arkTSMessageHandler :
         package->createArkTSMessageHandlers()) {
      arkTSMessageHandlers.push_back(arkTSMessageHandler);
    }
  }

  auto turboModuleFactory = TurboModuleFactory(
      env,
      arkTsTurboModuleProviderRef,
      std::move(componentJSIBinderByName),
      taskExecutor,
      std::move(turboModuleFactoryDelegates));
  auto mutationsToNapiConverter = std::make_shared<MutationsToNapiConverter>(
      std::move(componentNapiBinderByName));
  auto mountingManager = std::make_shared<MountingManager>(
      taskExecutor,
      shadowViewRegistry,
      [mutationsListener = std::move(mutationsListener),
       mutationsToNapiConverter](
          facebook::react::ShadowViewMutationList mutations) {
        mutationsListener(*mutationsToNapiConverter, mutations);
      },
      [weakExecutor = std::weak_ptr(taskExecutor),
       commandDispatcher = commandDispatcher](
          auto tag, auto commandName, auto args) {
        if (auto taskExecutor = weakExecutor.lock()) {
          taskExecutor->runTask(
              TaskThread::MAIN,
              [tag,
               commandDispatcher,
               commandName = std::move(commandName),
               args = std::move(args)]() {
                commandDispatcher(tag, commandName, args);
              });
        }
      });
  auto schedulerDelegateArkTS =
      std::make_unique<SchedulerDelegateArkTS>(mountingManager, arkTSChannel);
  if (shouldUseCAPIArchitecture) {
#ifdef C_API_ARCH
    auto componentInstanceDependencies =
        std::make_shared<ComponentInstance::Dependencies>();
    componentInstanceDependencies->arkTSChannel = arkTSChannel;
    auto customComponentArkUINodeFactory =
        std::make_shared<CustomComponentArkUINodeHandleFactory>(
            env, frameNodeFactoryRef, taskExecutor);
    auto componentInstanceFactory = std::make_shared<ComponentInstanceFactory>(
        componentInstanceFactoryDelegates,
        componentInstanceDependencies,
        customComponentArkUINodeFactory);
    auto componentInstanceRegistry =
        std::make_shared<ComponentInstanceRegistry>();
    auto schedulerDelegateCAPI = std::make_unique<SchedulerDelegateCAPI>(
        taskExecutor,
        componentInstanceRegistry,
        componentInstanceFactory,
        std::move(schedulerDelegateArkTS),
        mountingManager);
    auto rnInstance = std::make_shared<RNInstanceCAPI>(
        id,
        contextContainer,
        std::move(turboModuleFactory),
        taskExecutor,
        componentDescriptorProviderRegistry,
        mutationsToNapiConverter,
        eventEmitRequestHandlers,
        globalJSIBinders,
        uiTicker,
        shadowViewRegistry,
        std::move(schedulerDelegateCAPI),
        std::move(arkTSMessageHandlers),
        arkTSChannel,
        componentInstanceRegistry,
        componentInstanceFactory,
        shouldEnableDebugger,
        shouldEnableBackgroundExecutor);
    componentInstanceDependencies->rnInstance = rnInstance;
    return rnInstance;
#else
    LOG(FATAL)
        << "The C_API architecture also needs to be enabled on the CPP side. Have you set the "
           "RNOH_C_API_ARCH=\"1\" environment variable, completely closed and reopened DevEco Studio and "
           "run Build > Clean Project?";
#endif
  }
  return std::make_shared<RNInstanceArkTS>(
      id,
      contextContainer,
      std::move(turboModuleFactory),
      taskExecutor,
      componentDescriptorProviderRegistry,
      mutationsToNapiConverter,
      eventEmitRequestHandlers,
      globalJSIBinders,
      uiTicker,
      shadowViewRegistry,
      arkTSChannel,
      std::move(schedulerDelegateArkTS),
      std::move(arkTSMessageHandlers),
      shouldEnableDebugger,
      shouldEnableBackgroundExecutor);
}
