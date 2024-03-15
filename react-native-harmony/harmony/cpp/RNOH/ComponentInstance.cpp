#include "ComponentInstance.h"
#include <glog/logging.h>

namespace rnoh {
    ComponentInstance::ComponentInstance(Context context)
        : m_tag(context.tag), m_componentHandle(context.componentHandle), m_deps(std::move(context.dependencies)) {}

    void ComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
        auto it = m_children.begin() + index;
        onChildInserted(childComponentInstance, index);
        childComponentInstance->setParent(shared_from_this());
        m_children.insert(it, std::move(childComponentInstance));
    }

    void ComponentInstance::removeChild(ComponentInstance::Shared childComponentInstance) {
        auto it = std::find(m_children.begin(), m_children.end(), childComponentInstance);
        if (it != m_children.end()) {
            auto childComponentInstance = std::move(*it);
            m_children.erase(it);
            onChildRemoved(childComponentInstance);
        }
    }
} // namespace rnoh
