#pragma once
#include "NativeNodeApi.h"
#include "ArkUINode.h"

namespace rnoh {

    class ScrollNode : public ArkUINode {
    public:
        ScrollNode() : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_SCROLL)) {}
    
        void insertChild(ArkUINode &child) {
            maybeThrow(NativeNodeApi::getInstance()->addChild(m_nodeHandle, child.getArkUINodeHandle()));
        }
    
        
    };

} // namespace rnoh