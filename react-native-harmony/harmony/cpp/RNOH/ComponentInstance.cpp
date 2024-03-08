#include "ComponentInstance.h"
#include <glog/logging.h>

namespace rnoh {
    ComponentInstance::ComponentInstance(Context context, facebook::react::Tag tag) : m_tag(tag) {}

    void ComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
        auto it = m_children.begin() + index;
        m_children.insert(it, std::move(childComponentInstance));
    }

    void ComponentInstance::removeChild(ComponentInstance::Shared childComponentInstance) {
        auto it = std::find(m_children.begin(), m_children.end(), childComponentInstance);
        if (it != m_children.end()) {
            m_children.erase(it);
        }
    }
} // namespace rnoh