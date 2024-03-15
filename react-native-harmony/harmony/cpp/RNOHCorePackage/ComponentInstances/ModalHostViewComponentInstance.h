#pragma once

#include "RNOH/arkui/StackNode.h"
#include "RNOH/arkui/NativeNodeApi.h"
#include "RNOH/arkui/ArkUIDialogHandler.h"
#include "RNOH/CppComponentInstance.h"
#include "react/renderer/components/modal/ModalHostViewShadowNode.h"

namespace rnoh {
class ModalHostViewComponentInstance : public CppComponentInstance<facebook::react::ModalHostViewShadowNode> {
  private:
    StackNode m_virtualNode;
    StackNode m_rootStackNode;
    ArkUIDialogHandler m_dialogHandler;

  public:
    ModalHostViewComponentInstance(Context context);

    void onPropsChanged(SharedConcreteProps const &props) override;

    void insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) override;

    void removeChild(ComponentInstance::Shared childComponentInstance) override;

    StackNode &getLocalRootArkUINode() override;
};
} // namespace rnoh