/**
 * Used only in C-API based Architecture.
 */
#pragma once
#include <memory>
#include <vector>
#include <react/renderer/core/ReactPrimitives.h>
#include "RNOH/ComponentInstance.h"

namespace rnoh {
    class ComponentInstanceFactoryDelegate {
    public:
        ComponentInstanceFactoryDelegate() = default;

        using Shared = std::shared_ptr<ComponentInstanceFactoryDelegate>;

        virtual ComponentInstance::Shared create(ComponentInstance::Context ctx) = 0;
    };


    class ComponentInstanceFactory {
    private:
        std::vector<ComponentInstanceFactoryDelegate::Shared> m_delegates;
        ComponentInstance::Dependencies::Shared m_dependencies;

    public:
        using Shared = std::shared_ptr<ComponentInstanceFactory>;

        ComponentInstanceFactory(std::vector<ComponentInstanceFactoryDelegate::Shared> delegates,
                                 ComponentInstance::Dependencies::Shared dependencies)
            : m_delegates(std::move(delegates)), m_dependencies(dependencies) {}

        ComponentInstance::Shared create(facebook::react::Tag tag, facebook::react::ComponentHandle componentHandle,
                                         std::string componentName) {
            for (auto &delegate : m_delegates) {
                ComponentInstance::Context ctx = {.tag = tag,
                                                  .componentHandle = componentHandle,
                                                  .componentName = componentName,
                                                  .dependencies = m_dependencies};
                auto componentInstance = delegate->create(ctx);
                if (componentInstance != nullptr) {
                    return componentInstance;
                }
            }
            return nullptr;
        }
    };
} // namespace rnoh