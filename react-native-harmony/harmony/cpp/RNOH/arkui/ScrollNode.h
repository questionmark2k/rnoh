#pragma once
#include "NativeNodeApi.h"
#include "ArkUINode.h"

namespace rnoh {

class ScrollNode : public ArkUINode {
  public:
    ScrollNode();
    ~ScrollNode() override;

    void insertChild(ArkUINode &child) {
        maybeThrow(NativeNodeApi::getInstance()->addChild(m_nodeHandle, child.getArkUINodeHandle()));
    }

    void onNodeEvent(ArkUI_NodeEvent *event) override;
};

} // namespace rnoh