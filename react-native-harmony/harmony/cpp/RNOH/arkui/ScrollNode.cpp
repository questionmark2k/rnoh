#include "ScrollNode.h"
#include <glog/logging.h>

namespace rnoh {
    ScrollNode::ScrollNode()
        : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_SCROLL)),
          m_childArkUINodeHandle(nullptr) {
        maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL, 0));
    }

    ScrollNode::~ScrollNode() {
        NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL);
    }

    void ScrollNode::onNodeEvent(ArkUI_NodeEvent *event) {
        if (event->kind == ArkUI_NodeEventType::NODE_SCROLL_EVENT_ON_SCROLL) {
            // TODO: implement scroll events
        }
    }

    facebook::react::Point ScrollNode::getScrollOffset() const {
        // an attempt was made: this returns garbage
        auto item = NativeNodeApi::getInstance()->getAttribute(m_nodeHandle, NODE_SCROLL_OFFSET);
        auto x = item->value[0].f32;
        auto y = item->value[1].f32;
        DLOG(INFO) << "Scroll offset: " << x << ", " << y;
        return Point{x, y};
    }

    void ScrollNode::insertChild(ArkUINode &child) {
        if (m_childArkUINodeHandle != nullptr) {
            LOG(FATAL) << "ScrollNode can only have one child";
        }
        maybeThrow(NativeNodeApi::getInstance()->addChild(m_nodeHandle, child.getArkUINodeHandle()));
        m_childArkUINodeHandle = child.getArkUINodeHandle();
    }

    void ScrollNode::removeChild(ArkUINode &child) {
        if (m_childArkUINodeHandle == nullptr) {
            LOG(FATAL) << "Tried to remove child before inserting one.";
        }
        NativeNodeApi::getInstance()->removeChild(m_nodeHandle, m_childArkUINodeHandle);
        m_childArkUINodeHandle = nullptr;
    }
} // namespace rnoh