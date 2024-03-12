#pragma once

#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/TextNode.h"
#include <react/renderer/components/text/ParagraphShadowNode.h>

namespace rnoh {
    class TextComponentInstance : public CppComponentInstance<facebook::react::ParagraphShadowNode> {
    private:
        TextNode m_textNode;

    public:
        TextComponentInstance(Context context, facebook::react::Tag tag);

        void onPropsChanged(std::shared_ptr<const facebook::react::ParagraphProps> const &props) override;

        void onStateChanged(std::shared_ptr<const facebook::react::ConcreteState<facebook::react::ParagraphState>> const &state) override;

        TextNode &getLocalRootArkUINode() override;
    };
} // namespace rnoh