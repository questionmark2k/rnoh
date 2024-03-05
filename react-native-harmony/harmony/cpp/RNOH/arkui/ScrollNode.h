#pragma once
#include "NativeNodeApi.h"
#include "ArkUINode.h"

namespace rnoh {

    class ScrollNode : public ArkUINode {
    protected:
        ArkUI_NodeHandle m_childArkUINodeHandle;

    public:
        ScrollNode();
        ~ScrollNode();

        void insertChild(ArkUINode &child);
        void removeChild(ArkUINode &child);
        void onNodeEvent(ArkUI_NodeEvent *event) override;
    };

} // namespace rnoh