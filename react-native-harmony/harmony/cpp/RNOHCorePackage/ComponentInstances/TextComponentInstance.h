#pragma once

#include "RNOH/ComponentInstance.h"
#include "RNOH/arkui/TextNode.h"

namespace rnoh {
    class TextComponentInstance : public ComponentInstance {
    private:
        TextNode m_textNode;

    public:
        TextComponentInstance(Context context, facebook::react::Tag tag);

        void setProps(facebook::react::Props::Shared props) override;

        void setState(facebook::react::State::Shared state) override;

        TextNode &getLocalRootArkUINode() override;
    };
} // namespace rnoh