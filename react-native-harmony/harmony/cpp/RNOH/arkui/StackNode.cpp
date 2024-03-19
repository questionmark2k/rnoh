#include "StackNode.h"

#include <memory>
#include "NativeNodeApi.h"
#include <glog/logging.h>

namespace rnoh {

    StackNode::StackNode()
        : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_STACK)),
          m_stackNodeDelegate(nullptr) {
        maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_CLICK, 0));
    }

    void StackNode::insertChild(ArkUINode &child, std::size_t index) {
        maybeThrow(NativeNodeApi::getInstance()->addChild(m_nodeHandle, child.getArkUINodeHandle()));
    }

    void StackNode::removeChild(ArkUINode &child) {
        maybeThrow(NativeNodeApi::getInstance()->removeChild(m_nodeHandle, child.getArkUINodeHandle()));
    }

    void StackNode::setStackNodeDelegate(StackNodeDelegate *stackNodeDelegate) {
        m_stackNodeDelegate = stackNodeDelegate;
    }

    void StackNode::onNodeEvent(ArkUI_NodeEvent *event) {
        if (event->kind == ArkUI_NodeEventType::NODE_ON_CLICK && event->componentEvent.data[3].i32 != 2) {
            onClick();
        }
    }

    void StackNode::onClick() {
        if (m_stackNodeDelegate != nullptr) {
            m_stackNodeDelegate->onClick();
        }
    }

    StackNode& StackNode::setMargin(float left, float top, float right, float bottom) {
        ArkUI_NumberValue value[] = {{.f32 = top}, {.f32 = right}, {.f32 = bottom}, {.f32 = left}};
        ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_MARGIN, &item));
        return *this;
    }


    StackNode::~StackNode() {
        NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_CLICK);
    }
} // namespace rnoh
