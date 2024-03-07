#pragma once
#include "NativeNodeApi.h"
#include "ArkUINode.h"
#include <react/renderer/graphics/Point.h>

namespace rnoh {

    class ScrollNode : public ArkUINode {
    protected:
        using Point = facebook::react::Point;

        ArkUI_NodeHandle m_childArkUINodeHandle;

    public:
        ScrollNode();
        ~ScrollNode() override;

        void insertChild(ArkUINode &child);
        void removeChild(ArkUINode &child);
        void onNodeEvent(ArkUI_NodeEvent *event) override;

        Point getScrollOffset() const;
    };

} // namespace rnoh