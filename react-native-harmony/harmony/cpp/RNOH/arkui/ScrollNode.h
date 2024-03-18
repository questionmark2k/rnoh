#pragma once
#include "NativeNodeApi.h"
#include "ArkUINode.h"
#include <react/renderer/graphics/Point.h>

namespace rnoh {
    class ScrollNodeDelegate {
    public:
        virtual ~ScrollNodeDelegate() = default;
        virtual void onScroll(){};
        virtual void onScrollStart(){};
        virtual void onScrollStop(){};
        virtual float onScrollFrameBegin(float offset, int32_t scrollState) { return offset; };
    };


    class ScrollNode : public ArkUINode {
    protected:
        using Point = facebook::react::Point;

        ArkUI_NodeHandle m_childArkUINodeHandle;
        ScrollNodeDelegate *m_scrollNodeDelegate;
        void setNestedScroll();

    public:
        ScrollNode();
        ~ScrollNode() override;

        void insertChild(ArkUINode &child);
        void removeChild(ArkUINode &child);
        void onNodeEvent(ArkUI_NodeEvent *event) override;

        Point getScrollOffset() const;
        void setScrollNodeDelegate(ScrollNodeDelegate *scrollNodeDelegate);
        ScrollNode &setHorizontal(bool horizontal);
        ScrollNode &setEnableScrollInteraction(bool enableScrollInteraction);
        ScrollNode &setFriction(float friction);
        ScrollNode &setEdgeEffect(bool bounces, bool alwaysBounces);
    };

} // namespace rnoh