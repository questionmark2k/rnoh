#include "ScrollNode.h"

#include <glog/logging.h>

namespace rnoh {
ScrollNode::ScrollNode() : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_SCROLL)) {
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL, 0));
}

ScrollNode::~ScrollNode() {
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL);
}

void ScrollNode::onNodeEvent(ArkUI_NodeEvent *event) {
    if (event->kind == ArkUI_NodeEventType::NODE_SCROLL_EVENT_ON_SCROLL) {
        DLOG(INFO) << "ScrollNode " << m_nodeHandle << " received scroll event";
    }
}
} // namespace rnoh