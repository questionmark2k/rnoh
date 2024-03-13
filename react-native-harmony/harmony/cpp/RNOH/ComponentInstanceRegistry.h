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
        std::mutex m_mtx;

    public:
        using Shared = std::shared_ptr<ComponentInstanceRegistry>;
    
        ~ComponentInstanceRegistry() {
            DLOG(INFO) << "~ComponentInstanceRegistry"; 
        }

        ComponentInstance::Shared findByTag(facebook::react::Tag tag) {
            std::lock_guard<std::mutex> lock(m_mtx);
            auto it = m_componentInstanceByTag.find(tag);
            if (it != m_componentInstanceByTag.end()) {
                return it->second;
            }
            return nullptr;
        }

        void insert(ComponentInstance::Shared componentInstance) {
            std::lock_guard<std::mutex> lock(m_mtx);
            auto tag = componentInstance->getTag();
            m_componentInstanceByTag.emplace(tag, std::move(componentInstance));
        }

        void deleteByTag(facebook::react::Tag tag) {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_componentInstanceByTag.erase(tag);
        }
    };
} // namespace rnoh