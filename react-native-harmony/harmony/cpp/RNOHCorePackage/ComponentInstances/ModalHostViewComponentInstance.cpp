#include "ModalHostViewComponentInstance.h"

#include <react/renderer/components/rncore/Props.h>
#include <glog/logging.h>

namespace rnoh {

ModalHostViewComponentInstance::ModalHostViewComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {
    m_virtualNode.setSize(facebook::react::Size{0, 0});
    m_dialogHandler.attachContent(m_rootStackNode);
    m_dialogHandler.show();
}

void ModalHostViewComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
    CppComponentInstance::onPropsChanged(props);
    if (!props) {
        return;
    }
    if (!m_props || props->animationType != m_props->animationType) {
        m_rootStackNode.setTransition(props->animationType);
    }
}

void ModalHostViewComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
    CppComponentInstance::insertChild(childComponentInstance, index);
    m_rootStackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
}

void ModalHostViewComponentInstance::removeChild(ComponentInstance::Shared childComponentInstance) {
    CppComponentInstance::removeChild(childComponentInstance);
    m_rootStackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
};

StackNode &ModalHostViewComponentInstance::getLocalRootArkUINode() { return m_virtualNode; }

} // namespace rnoh
