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
  std::unordered_map<facebook::react::Tag, ComponentInstance::Shared>
      m_componentInstanceByTag;
  std::unordered_map<std::string, facebook::react::Tag> m_tagById;
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

  std::optional<facebook::react::Tag> findTagById(const std::string& id) {
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_tagById.find(id);
    if (it != m_tagById.end()) {
      return it->second;
    }
    return std::nullopt;
  }

  void insert(ComponentInstance::Shared componentInstance) {
    std::lock_guard<std::mutex> lock(m_mtx);
    auto tag = componentInstance->getTag();
    m_componentInstanceByTag.emplace(tag, std::move(componentInstance));
  }

  void updateTagById(
      facebook::react::Tag tag,
      const std::string& id,
      const std::string prevId) {
    if (!prevId.empty() && m_tagById.find(prevId) != m_tagById.end()) {
      m_tagById.erase(prevId);
    }
    if (!id.empty()) {
      m_tagById.emplace(id, tag);
    }
  }

  void deleteByTag(facebook::react::Tag tag) {
    std::lock_guard<std::mutex> lock(m_mtx);
    auto tagAndComponentInstance = m_componentInstanceByTag.find(tag);
    if (tagAndComponentInstance == m_componentInstanceByTag.end()) {
      return;
    }
    auto componentInstance = tagAndComponentInstance->second;
    auto componentInstanceId = componentInstance->getId();
    if (!componentInstanceId.empty()) {
      m_tagById.erase(componentInstanceId);
    }
    m_componentInstanceByTag.erase(tag);
  }
};
} // namespace rnoh