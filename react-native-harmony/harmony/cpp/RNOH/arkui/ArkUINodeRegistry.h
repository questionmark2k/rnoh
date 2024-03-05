#pragma once

#include <unordered_map>
#include <arkui/native_node.h>

namespace rnoh {

class ArkUINode;

class ArkUINodeRegistry {
public:
    static ArkUINodeRegistry &getInstance();

    void registerNode(ArkUINode *node);
    void unregisterNode(ArkUINode *node);

private:
    ArkUINodeRegistry();

    void receiveEvent(ArkUI_NodeEvent *event);

    std::unordered_map<ArkUI_NodeHandle, ArkUINode *> m_nodeByHandle;
};

} // namespace rnoh