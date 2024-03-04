/**
 * Used only in C-API based Architecture.
 */
#pragma once
#include <memory>
#include <vector>
#include <react/renderer/core/ReactPrimitives.h>
#include "RNOH/ComponentInstance.h"

namespace rnoh {
    struct ComponentInstanceFactoryContext {
        std::string componentName;
        facebook::react::Tag tag;
    };

    class ComponentInstanceFactoryDelegate {
    protected:
        ComponentInstance::Context m_ctx;

    public:
        ComponentInstanceFactoryDelegate(ComponentInstance::Context ctx) : m_ctx(ctx) {}

        using Shared = std::shared_ptr<ComponentInstanceFactoryDelegate>;

        virtual ComponentInstance::Shared create(ComponentInstanceFactoryContext ctx) = 0;
    };


    class ComponentInstanceFactory {
    private:
        std::vector<ComponentInstanceFactoryDelegate::Shared> m_delegates;

    public:
        using Shared = std::shared_ptr<ComponentInstanceFactory>;

        ComponentInstanceFactory(std::vector<ComponentInstanceFactoryDelegate::Shared> delegates)
            : m_delegates(std::move(delegates)) {}

        ComponentInstance::Shared create(ComponentInstanceFactoryContext ctx) {
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