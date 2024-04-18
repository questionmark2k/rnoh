#pragma once

#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/LoadingProgressNode.h"
#include "react/renderer/components/rncore/ShadowNodes.h"

namespace rnoh {
class ActivityIndicatorComponentInstance
    : public CppComponentInstance<
          facebook::react::ActivityIndicatorViewShadowNode> {
 private:
  LoadingProgressNode m_loadingProgressNode;

 public:
  ActivityIndicatorComponentInstance(Context context);

  void onPropsChanged(SharedConcreteProps const& props) override;

  LoadingProgressNode& getLocalRootArkUINode() override;
};
} // namespace rnoh