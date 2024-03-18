#include "ScrollNode.h"
#include <glog/logging.h>

namespace rnoh {
    ScrollNode::ScrollNode()
        : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_SCROLL)),
          m_childArkUINodeHandle(nullptr), m_scrollNodeDelegate(nullptr) {
        maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL, 0));
        maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL_START, 0));
        maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL_STOP, 0));
        maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL_FRAME_BEGIN, 0));
        setNestedScroll();
    }

    ScrollNode::~ScrollNode() {
        NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL);
        NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL_START);
        NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL_STOP);
        NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_SCROLL_EVENT_ON_SCROLL_FRAME_BEGIN);
    }

    void ScrollNode::onNodeEvent(ArkUI_NodeEvent *event) {
        if (event->kind == ArkUI_NodeEventType::NODE_SCROLL_EVENT_ON_SCROLL) {
            if (m_scrollNodeDelegate != nullptr) {
                m_scrollNodeDelegate->onScroll();
            }
        } else if (event->kind == ArkUI_NodeEventType::NODE_SCROLL_EVENT_ON_SCROLL_START) {
            if (m_scrollNodeDelegate != nullptr) {
                m_scrollNodeDelegate->onScrollStart();
            }
        } else if (event->kind == ArkUI_NodeEventType::NODE_SCROLL_EVENT_ON_SCROLL_STOP) {
            if (m_scrollNodeDelegate != nullptr) {
                m_scrollNodeDelegate->onScrollStop();
            }
        } else if (event->kind == ArkUI_NodeEventType::NODE_SCROLL_EVENT_ON_SCROLL_FRAME_BEGIN) {
            if (m_scrollNodeDelegate != nullptr) {
                auto remainingOffset = m_scrollNodeDelegate->onScrollFrameBegin(event->componentEvent.data[0].f32, event->componentEvent.data[1].i32);
                event->componentEvent.data[0].f32 = remainingOffset;        
            }
        }
    }

    facebook::react::Point ScrollNode::getScrollOffset() const {
        auto item = NativeNodeApi::getInstance()->getAttribute(m_nodeHandle, NODE_SCROLL_OFFSET);
        facebook::react::Float x = item->value[0].i32;
        facebook::react::Float y = item->value[1].i32;
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

    void ScrollNode::setScrollNodeDelegate(ScrollNodeDelegate *scrollNodeDelegate) {
        m_scrollNodeDelegate = scrollNodeDelegate;
    }

    void ScrollNode::setNestedScroll() {
        ArkUI_NumberValue value[] = {{.i32 = ArkUI_ScrollNestedMode::ARKUI_SCROLL_NESTED_MODE_SELF_FIRST}, {.i32 = ArkUI_ScrollNestedMode::ARKUI_SCROLL_NESTED_MODE_SELF_FIRST}};
        ArkUI_AttributeItem item = {value, sizeof(value)/sizeof(ArkUI_NumberValue)};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_SCROLL_NESTED_SCROLL, &item));
    }

    ScrollNode& ScrollNode::setHorizontal(bool horizontal) {
        auto scrollDirection = horizontal ? ArkUI_ScrollDirection::ARKUI_SCROLL_DIRECTION_HORIZONTAL : ArkUI_ScrollDirection::ARKUI_SCROLL_DIRECTION_VERTICAL;
        ArkUI_NumberValue value[] = {{.i32 =  scrollDirection}};
        ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_SCROLL_SCROLL_DIRECTION, &item));
        return *this;
    }

    ScrollNode& ScrollNode::setEnableScrollInteraction(bool enableScrollInteraction) {
        ArkUI_NumberValue value[] = {{.i32 = enableScrollInteraction}};
        ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_SCROLL_ENABLE_SCROLL_INTERACTION, &item));
        return *this;
    }

    ScrollNode& ScrollNode::setFriction(float friction) {
        ArkUI_NumberValue value[] = {{.f32 = friction}};
        ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
        maybeThrow(
            NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_SCROLL_FRICTION, &item));
        return *this;
    }

    ScrollNode& ScrollNode::setEdgeEffect(bool bounces, bool alwaysBounce) {
        ArkUI_EdgeEffect edgeEffect = bounces ? ArkUI_EdgeEffect::ARKUI_EDGE_EFFECT_SPRING : ArkUI_EdgeEffect::ARKUI_EDGE_EFFECT_NONE;
        ArkUI_NumberValue value[] = {{.i32 = edgeEffect}, {.i32 = alwaysBounce}};
        ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_SCROLL_EDGE_EFFECT, &item));
        return *this;
    }
} // namespace rnoh