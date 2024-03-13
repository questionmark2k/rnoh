#include "ViewComponentInstance.h"

namespace rnoh {

    ViewComponentInstance::ViewComponentInstance(Context context)
        : CppComponentInstance(std::move(context)) {}

    void ViewComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
        CppComponentInstance::insertChild(childComponentInstance, index);
        m_stackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
    }

    void ViewComponentInstance::removeChild(ComponentInstance::Shared childComponentInstance) {
        CppComponentInstance::removeChild(childComponentInstance);
        m_stackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
    };

    StackNode &ViewComponentInstance::getLocalRootArkUINode() { return m_stackNode; }
} // namespace rnoh
