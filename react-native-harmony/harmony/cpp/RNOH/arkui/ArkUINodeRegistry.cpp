#include "ArkUINodeRegistry.h"

#include <glog/logging.h>
#include "ArkUINode.h"
#include "NativeNodeApi.h"

namespace rnoh {

ArkUINodeRegistry &ArkUINodeRegistry::getInstance() {
    static ArkUINodeRegistry instance;
    return instance;
}

void ArkUINodeRegistry::registerNode(ArkUINode *node) {
    auto [_it, inserted] = m_nodeByHandle.emplace(node->getArkUINodeHandle(), node);
    if (!inserted) {
        LOG(WARNING) << "Node with handle: " << node->getArkUINodeHandle() << " was already registered";
    }
}

void ArkUINodeRegistry::unregisterNode(ArkUINode *node) {
    auto it = m_nodeByHandle.find(node->getArkUINodeHandle());
    if (it == m_nodeByHandle.end()) {
        LOG(WARNING) << "Node with handle: " << node->getArkUINodeHandle() << " not found";
        return;
    }

    m_nodeByHandle.erase(it);
}

void ArkUINodeRegistry::registerTouchHandler(ArkUINode *node, TouchEventHandler *touchEventHandler) {
    DLOG(INFO) << "Registering touch handler for node handle " << node->getArkUINodeHandle();
    auto [_it, inserted] = m_touchHandlerByNodeHandle.emplace(node->getArkUINodeHandle(), touchEventHandler);
    if (!inserted) {
        LOG(WARNING) << "Touch handler for node handle: " << node->getArkUINodeHandle() << " was already registered";
    }
}

void ArkUINodeRegistry::unregisterTouchHandler(ArkUINode *node) {
    DLOG(INFO) << "Unregistering touch handler for node handle " << node->getArkUINodeHandle();
    auto it = m_touchHandlerByNodeHandle.find(node->getArkUINodeHandle());
    if (it == m_touchHandlerByNodeHandle.end()) {
        LOG(WARNING) << "Touch handler for node handle: " << node->getArkUINodeHandle() << " not found";
        return;
    }
    m_touchHandlerByNodeHandle.erase(it);
}

ArkUINodeRegistry::ArkUINodeRegistry() {
    NativeNodeApi::getInstance()->registerNodeEventReceiver([](ArkUI_NodeEvent *event) {
        ArkUINodeRegistry::getInstance().receiveEvent(event);
    });
}

void ArkUINodeRegistry::receiveEvent(ArkUI_NodeEvent *event) {
    if (event->kind == ArkUI_NodeEventType::NODE_TOUCH_EVENT) {
        auto it = m_touchHandlerByNodeHandle.find(event->node);
        if (it == m_touchHandlerByNodeHandle.end()) {
            LOG(WARNING) << "Touch event for node with handle: " << event->node << " not found";
            return;
        }

        it->second->onTouchEvent(event->touchEvent);
        return;
    }

    auto it = m_nodeByHandle.find(event->node);
    if (it == m_nodeByHandle.end()) {
        LOG(WARNING) << "Node with handle: " << event->node << " not found";
        return;
    }

    it->second->onNodeEvent(event);
}

} // namespace rnoh