#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <react/renderer/mounting/ShadowViewMutation.h>
#include <react/renderer/components/image/ImageComponentDescriptor.h>
#include <react/renderer/components/text/ParagraphComponentDescriptor.h>
#include "RNOH/ArkJS.h"
#include "RNOH/RNInstance.h"
#include "RNOH/MutationsToNapiConverter.h"
#include "RNOH/TurboModuleFactory.h"
#include "RNOH/ArkTSTurboModule.h"
#include "RNOH/PackageProvider.h"
#include "RNOHCorePackage/RNOHCorePackage.h"
#include "RNOH/EventEmitRequestHandler.h"
#include "RNOH/TextMeasurer.h"
#include "RNOH/UITicker.h"
#include "RNOH/ArkTSChannel.h"
#include "RNOH/FeatureFlagRegistry.h"

using namespace rnoh;

std::unique_ptr<RNInstance> createRNInstance(int id, napi_env env, napi_ref arkTsTurboModuleProviderRef,
                                             MutationsListener &&mutationsListener,
                                             MountingManager::CommandDispatcher &&commandDispatcher,
                                             napi_ref measureTextFnRef, napi_ref napiEventDispatcherRef,
                                             FeatureFlagRegistry::Shared featureFlagRegistry, UITicker::Shared uiTicker,
                                             bool shouldEnableDebugger, bool shouldEnableBackgroundExecutor) {
    std::shared_ptr<TaskExecutor> taskExecutor = std::make_shared<TaskExecutor>(env, shouldEnableBackgroundExecutor);
    auto arkTSChannel = std::make_shared<ArkTSChannel>(taskExecutor, ArkJS(env), napiEventDispatcherRef);

    taskExecutor->setExceptionHandler([weakExecutor = std::weak_ptr(taskExecutor),
                                       weakChannel = std::weak_ptr(arkTSChannel)](std::exception const &e) {
        auto executor = weakExecutor.lock();
        if (executor == nullptr) {
            return;
        }

        auto payload = folly::dynamic::object();
        if (auto *jsError = dynamic_cast<const facebook::jsi::JSError *>(&e)) {
            payload("message", jsError->getMessage())("stack", jsError->getStack());
        } else {
            payload("message", e.what());
        }

        try {
            std::rethrow_if_nested(e);
        } catch (std::exception const &nested) {
            payload("nested", nested.what());
        }

        executor->runTask(TaskThread::MAIN, [payload = folly::dynamic(std::move(payload)), weakChannel]() {
            auto channel = weakChannel.lock();
            if (channel != nullptr) {
                channel->postMessage("RNOH_ERROR", std::move(payload));
            }
        });
    });

    auto contextContainer = std::make_shared<facebook::react::ContextContainer>();
    auto textMeasurer = std::make_shared<TextMeasurer>(env, measureTextFnRef, taskExecutor, featureFlagRegistry);
    auto shadowViewRegistry = std::make_shared<ShadowViewRegistry>();
    contextContainer->insert("textLayoutManagerDelegate", textMeasurer);
    PackageProvider packageProvider;
    auto packages = packageProvider.getPackages({});
    packages.insert(packages.begin(),
                    std::make_shared<RNOHCorePackage>(Package::Context{.shadowViewRegistry = shadowViewRegistry}));

    auto componentDescriptorProviderRegistry = std::make_shared<facebook::react::ComponentDescriptorProviderRegistry>();
    std::vector<std::shared_ptr<TurboModuleFactoryDelegate>> turboModuleFactoryDelegates;
    ComponentJSIBinderByString componentJSIBinderByName = {};
    GlobalJSIBinders globalJSIBinders = {};
    ComponentNapiBinderByString componentNapiBinderByName = {};
    EventEmitRequestHandlers eventEmitRequestHandlers = {};

    for (auto &package : packages) {
        auto turboModuleFactoryDelegate = package->createTurboModuleFactoryDelegate();
        if (turboModuleFactoryDelegate != nullptr) {
            turboModuleFactoryDelegates.push_back(std::move(turboModuleFactoryDelegate));
        }
        for (auto componentDescriptorProvider : package->createComponentDescriptorProviders()) {
            componentDescriptorProviderRegistry->add(componentDescriptorProvider);
        }
        for (auto [name, componentJSIBinder] : package->createComponentJSIBinderByName()) {
            componentJSIBinderByName.insert({name, componentJSIBinder});
        };
        for (auto [name, componentNapiBinder] : package->createComponentNapiBinderByName()) {
            componentNapiBinderByName.insert({name, componentNapiBinder});
        };
        auto packageGlobalJSIBinders = package->createGlobalJSIBinders();
        globalJSIBinders.insert(globalJSIBinders.end(), std::make_move_iterator(packageGlobalJSIBinders.begin()),
                                std::make_move_iterator(packageGlobalJSIBinders.end()));
        auto packageEventEmitRequestHandlers = package->createEventEmitRequestHandlers();
        eventEmitRequestHandlers.insert(eventEmitRequestHandlers.end(),
                                        std::make_move_iterator(packageEventEmitRequestHandlers.begin()),
                                        std::make_move_iterator(packageEventEmitRequestHandlers.end()));
    }

    auto turboModuleFactory = TurboModuleFactory(env, arkTsTurboModuleProviderRef, std::move(componentJSIBinderByName),
                                                 taskExecutor, std::move(turboModuleFactoryDelegates));
    auto mutationsToNapiConverter = std::make_shared<MutationsToNapiConverter>(std::move(componentNapiBinderByName));
    auto schedulerDelegate = std::make_unique<SchedulerDelegate>(
        MountingManager(
            taskExecutor, shadowViewRegistry,
            [mutationsListener = std::move(mutationsListener),
             mutationsToNapiConverter](facebook::react::ShadowViewMutationList mutations) {
                mutationsListener(*mutationsToNapiConverter, mutations);
            },
            [weakExecutor = std::weak_ptr(taskExecutor),
             commandDispatcher = commandDispatcher](auto tag, auto commandName, auto args) {
                if (auto taskExecutor = weakExecutor.lock()) {
                    taskExecutor->runTask(TaskThread::MAIN,
                                          [tag, commandDispatcher, commandName = std::move(commandName),
                                           args = std::move(args)]() { commandDispatcher(tag, commandName, args); });
                }
            }),
        arkTSChannel);
    return std::make_unique<RNInstance>(
        id, contextContainer, std::move(turboModuleFactory), taskExecutor, componentDescriptorProviderRegistry,
        mutationsToNapiConverter, eventEmitRequestHandlers, globalJSIBinders,
        uiTicker, shadowViewRegistry, std::move(schedulerDelegate), shouldEnableDebugger,
        shouldEnableBackgroundExecutor);
}
