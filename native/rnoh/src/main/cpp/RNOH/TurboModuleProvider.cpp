#include "RNOH/TurboModuleProvider.h"

#include <ReactCommon/TurboModuleBinding.h>
#include <ReactCommon/LongLivedObject.h>
#include "RNOH/LogSink.h"


using namespace rnoh;
using namespace facebook;

TurboModuleProvider::TurboModuleProvider(std::shared_ptr<react::CallInvoker> jsInvoker,
                                                 TurboModuleFactory &&turboModuleFactory)
    : m_jsInvoker(jsInvoker),
      m_createTurboModule([factory = std::move(turboModuleFactory), env = TurboModuleFactory::arkTsTurboModuleFactoryEnv](
                              std::string const &moduleName,
                              std::shared_ptr<react::CallInvoker> jsInvoker) -> std::shared_ptr<react::TurboModule> {
          return factory.create(jsInvoker, moduleName);
      }) {}

void TurboModuleProvider::installJSBindings(react::RuntimeExecutor runtimeExecutor) {
    if (!runtimeExecutor) {
        LOG(ERROR) << "Turbo Modules couldn't be installed. Invalid RuntimeExecutor.";
        return;
    }
    auto turboModuleProvider = [self = this->shared_from_this()](std::string const &moduleName) {
        auto turboModule = self->getTurboModule(moduleName);
        return turboModule;
    };
    runtimeExecutor(
        [turboModuleProvider = std::move(turboModuleProvider)](facebook::jsi::Runtime &runtime) {
            react::TurboModuleBinding::install(runtime,
                                        std::move(turboModuleProvider),
                                        react::TurboModuleBindingMode::HostObject,
                                        nullptr);
        });
}

std::shared_ptr<react::TurboModule> TurboModuleProvider::getTurboModule(std::string const &moduleName) {
    if (m_cache.contains(moduleName)) {
        LOG(INFO) << "Cache hit. Providing '" << moduleName << "' Turbo Module";
        return m_cache[moduleName];
    }
    auto turboModule = m_createTurboModule(moduleName, m_jsInvoker);
    if (turboModule != nullptr) {
        m_cache[moduleName] = turboModule;
        return turboModule;
    }
    LOG(ERROR) << "Couldn't provide turbo module \" " << moduleName << "\"";
    return nullptr;
}