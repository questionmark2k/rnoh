#include "ComponentInstance.h"
#include <glog/logging.h>

namespace rnoh {
ComponentInstance::ComponentInstance(Context ctx)
    : m_tag(ctx.tag),
      m_componentHandle(ctx.componentHandle),
      m_componentName(std::move(ctx.componentName)),
      m_deps(std::move(ctx.dependencies)) {}

void ComponentInstance::insertChild(
    ComponentInstance::Shared childComponentInstance,
    std::size_t index) {
  auto it = m_children.begin() + index;
  onChildInserted(childComponentInstance, index);
  childComponentInstance->setParent(shared_from_this());
  m_children.insert(it, std::move(childComponentInstance));
}

void ComponentInstance::removeChild(
    ComponentInstance::Shared childComponentInstance) {
  auto it =
      std::find(m_children.begin(), m_children.end(), childComponentInstance);
  if (it != m_children.end()) {
    auto childComponentInstance = std::move(*it);
    m_children.erase(it);
    onChildRemoved(childComponentInstance);
  }
}
} // namespace rnoh
