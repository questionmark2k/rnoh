#include "ModalHostViewComponentInstance.h"

#include <react/renderer/components/rncore/Props.h>
#include <glog/logging.h>
#include "RNOH/ArkTSBridge.h"

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

void ModalHostViewComponentInstance::onStateChanged(SharedConcreteState const &state) {
    CppComponentInstance::onStateChanged(state);
    if (!m_state) {
        // set screen size the first time the component is initialized
        auto displayMetrics = ArkTSBridge::getInstance().getDisplayMetrics();
        auto screenMetrics = displayMetrics.screenPhysicalPixels;
        facebook::react::Size screenSize = {.width = screenMetrics.width / screenMetrics.scale, .height = screenMetrics.height / screenMetrics.scale};
        state->updateState({ screenSize });
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
    // only show modal after the screen size has been set and processed by RN
    auto isScreenSizeSet = m_state && m_state->getData().screenSize.height != 0 && m_state->getData().screenSize.width != 0;
    auto shouldShowDialog = !m_dialogHandler.isShow() && isScreenSizeSet;
    if (shouldShowDialog) { showDialog(); }
    CppComponentInstance::finalizeUpdates();
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
