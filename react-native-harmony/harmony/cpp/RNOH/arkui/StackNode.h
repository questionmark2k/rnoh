#pragma once

#include "ArkUINode.h"

namespace rnoh {
class StackNodeDelegate {
  public:
    virtual ~StackNodeDelegate() = default;
    virtual void onClick(){};
};

class StackNode : public ArkUINode {
protected:
    StackNodeDelegate *m_stackNodeDelegate;
public:
    StackNode();
    ~StackNode() override;

    void insertChild(ArkUINode &child, std::size_t index);
    void removeChild(ArkUINode &child);
    void onNodeEvent(ArkUI_NodeEvent *event) override;
    void onClick();
    void setStackNodeDelegate(StackNodeDelegate *stackNodeDelegate);
};

} // namespace rnoh