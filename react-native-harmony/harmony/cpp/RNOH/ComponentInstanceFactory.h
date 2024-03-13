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

    public:
        using Shared = std::shared_ptr<ComponentInstanceFactory>;

        ComponentInstanceFactory(std::vector<ComponentInstanceFactoryDelegate::Shared> delegates)
            : m_delegates(std::move(delegates)) {}

        ComponentInstance::Shared create(ComponentInstance::Context ctx) {
            for (auto &delegate : m_delegates) {
                auto componentInstance = delegate->create(ctx);
                if (componentInstance != nullptr) {
                    return componentInstance;
                }
            }
            return nullptr;
        }
    };
} // namespace rnoh