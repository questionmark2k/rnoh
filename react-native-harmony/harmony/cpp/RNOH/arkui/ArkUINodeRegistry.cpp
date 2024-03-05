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

ArkUINodeRegistry::ArkUINodeRegistry() {
    NativeNodeApi::getInstance()->registerNodeEventReceiver([](ArkUI_NodeEvent *event) {
        ArkUINodeRegistry::getInstance().receiveEvent(event);
    });
}

void ArkUINodeRegistry::receiveEvent(ArkUI_NodeEvent *event) {
    auto it = m_nodeByHandle.find(event->node);
    if (it == m_nodeByHandle.end()) {
        LOG(WARNING) << "Node with handle: " << event->node << " not found";
        return;
    }

    it->second->onNodeEvent(event);
}

} // namespace rnoh