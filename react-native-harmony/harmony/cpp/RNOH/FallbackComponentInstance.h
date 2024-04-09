#pragma once
#include <react/renderer/components/view/ViewShadowNode.h>
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/ArkUINode.h"

namespace rnoh {
/**
 * This ComponentInstance is created when no other ComponentInstance was found.
 * It is used for backward compatibility reasons with ArkTS-based architecture.
 */
class FallbackComponentInstance
    : public CppComponentInstance<facebook::react::ViewShadowNode> {
 private:
  std::unique_ptr<ArkUINode> m_arkUINode;

 public:
  FallbackComponentInstance(Context ctx, std::unique_ptr<ArkUINode> arkUINode)
      : CppComponentInstance(ctx), m_arkUINode(std::move(arkUINode)){};

  ArkUINode& getLocalRootArkUINode() override {
    return *m_arkUINode;
  };

  void setLayout(facebook::react::LayoutMetrics layoutMetrics) override {
    // Attributes are not set on the C++ side to prevent conflicts with ArkUI
    // front-end state management.
  }

 protected:
  void onPropsChanged(const SharedConcreteProps& props) override {
    // NOOP: Props are set on ArkTS side.
  }

  void onChildInserted(
      ComponentInstance::Shared const& childComponentInstance,
      std::size_t index) override {
    // The child node is added on the ArkTS side.
  }
};
} // namespace rnoh
