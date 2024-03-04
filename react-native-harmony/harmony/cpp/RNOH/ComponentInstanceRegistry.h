/**
 * Used only in C-API based Architecture.
 */
#pragma once
#include <memory>

#include <react/renderer/core/ReactPrimitives.h>
#include "RNOH/ComponentInstance.h"

namespace rnoh {
    class ComponentInstanceRegistry {
    private:
        std::unordered_map<facebook::react::Tag, ComponentInstance::Shared> m_componentInstanceByTag;

    public:
        using Shared = std::shared_ptr<ComponentInstanceRegistry>;

        ComponentInstance::Shared findByTag(facebook::react::Tag tag) {
            auto it = m_componentInstanceByTag.find(tag);
            if (it != m_componentInstanceByTag.end()) {
                return it->second;
            }
            return nullptr;
        }

        void insert(ComponentInstance::Shared componentInstance) { m_componentInstanceByTag[componentInstance->getTag()] = componentInstance; }

        void deleteByTag(facebook::react::Tag tag) { m_componentInstanceByTag.erase(tag); }
    };
} // namespace rnoh