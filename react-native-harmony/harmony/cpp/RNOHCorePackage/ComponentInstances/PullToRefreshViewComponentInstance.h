#pragma once
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/RefreshNode.h"
#include "RNOH/arkui/LoadingProgressNode.h"
#include <react/renderer/components/rncore/EventEmitters.h>
#include "react/renderer/components/rncore/ShadowNodes.h"

namespace rnoh {

    class PullToRefreshViewComponentInstance : public CppComponentInstance<facebook::react::PullToRefreshViewShadowNode>, public RefreshNodeDelegate {
    private:
        RefreshNode m_refreshNode;
        LoadingProgressNode m_loadingProgressNode;
    
    public:
        PullToRefreshViewComponentInstance(Context context);
        
        RefreshNode &getLocalRootArkUINode() override;
        
        void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
        void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;
        void onPropsChanged(SharedConcreteProps const &props) override;
        void onRefresh() override;
    };
} // namespace rnoh