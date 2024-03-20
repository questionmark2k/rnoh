#include "ModalHostViewComponentInstance.h"

#include <react/renderer/components/rncore/Props.h>
#include <glog/logging.h>

namespace rnoh {

ModalHostViewComponentInstance::ModalHostViewComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {
    m_virtualNode.setSize(facebook::react::Size{0, 0});
    m_dialogHandler.setDialogDelegate(this);
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

void ModalHostViewComponentInstance::onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) {
    CppComponentInstance::onChildInserted(childComponentInstance, index);
    m_rootStackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
}

void ModalHostViewComponentInstance::onChildRemoved(ComponentInstance::Shared const &childComponentInstance) {
    CppComponentInstance::onChildRemoved(childComponentInstance);
    m_rootStackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
};

void ModalHostViewComponentInstance::finalizeUpdates() {
    if (!m_dialogHandler.isShow()) {
        showDialog();
    }
    ComponentInstance::finalizeUpdates();
}

void ModalHostViewComponentInstance::showDialog() {
    m_dialogHandler.setContent(m_rootStackNode);
    m_dialogHandler.show();
}

void ModalHostViewComponentInstance::onShow() {
    if (m_eventEmitter != nullptr) {
        m_eventEmitter->onShow({});
    }
}

void ModalHostViewComponentInstance::onRequestClose() {
    if (m_eventEmitter != nullptr) {
        m_eventEmitter->onRequestClose({});
    }
}

StackNode &ModalHostViewComponentInstance::getLocalRootArkUINode() { return m_virtualNode; }

} // namespace rnoh
