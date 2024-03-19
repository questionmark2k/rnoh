#pragma once

#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/StackNode.h"
#include "RNOH/arkui/TextNode.h"
#include "RNOH/arkui/SpanNode.h"
#include "TextConversions.h"
#include <react/renderer/components/text/ParagraphShadowNode.h>

namespace rnoh {
    class TextComponentInstance : public CppComponentInstance<facebook::react::ParagraphShadowNode> {
    private:
        TextNode m_textNode;
        StackNode m_stackNode;
        uint32_t m_childIndex = 0;
        std::vector<std::shared_ptr<ArkUINode>> m_children;

    public:
        TextComponentInstance(Context context);
        ~TextComponentInstance();
        StackNode &getLocalRootArkUINode() override;

    protected:
        void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
        void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;
        void onPropsChanged(std::shared_ptr<const facebook::react::ParagraphProps> const &props) override;
        void onStateChanged(std::shared_ptr<const facebook::react::ConcreteState<facebook::react::ParagraphState>> const
                                &state) override;

    private:
        void setTextAttributes(const facebook::react::TextAttributes &textAttributes);
        void setFragment(const facebook::react::AttributedString::Fragment &fragment,
                         std::shared_ptr<SpanNode> spanNode, uint32_t index);
        void setImageSpanSize(const facebook::react::Size &imageSize, std::shared_ptr<ImageSpanNode> imageSpanNode);
        void setParagraphAttributes(const facebook::react::ParagraphAttributes &paragraphAttributes);
        std::string stringCapitalize(const std::string &strInput);
    };
} // namespace rnoh