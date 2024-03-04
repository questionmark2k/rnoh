#include "ViewComponentInstance.h"

namespace rnoh {

ViewComponentInstance::ViewComponentInstance(Context context, facebook::react::Tag tag)
            : ComponentInstance(std::move(context), tag) {}

void ViewComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
    m_stackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
}

StackNode &ViewComponentInstance::getLocalRootArkUINode() { return m_stackNode; }

} // namespace rnoh