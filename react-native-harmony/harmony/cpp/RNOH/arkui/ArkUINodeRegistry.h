#pragma once

#include <unordered_map>
#include <functional>
#include <arkui/native_node.h>

namespace rnoh {

class ArkUINode;

class TouchEventHandler {
  public:
    virtual void onTouchEvent(ArkUI_NodeTouchEvent event) = 0;
    virtual ~TouchEventHandler() = default;
};

class ArkUINodeRegistry {
  public:
    static ArkUINodeRegistry &getInstance();

    void registerNode(ArkUINode *node);
    void unregisterNode(ArkUINode *node);

    void registerTouchHandler(ArkUINode *node, TouchEventHandler *touchEventHandler);
    void unregisterTouchHandler(ArkUINode *node);

  private:
    ArkUINodeRegistry();

    void receiveEvent(ArkUI_NodeEvent *event);

    std::unordered_map<ArkUI_NodeHandle, ArkUINode *> m_nodeByHandle;
    std::unordered_map<ArkUI_NodeHandle, TouchEventHandler *> m_touchHandlerByNodeHandle;
};

} // namespace rnoh