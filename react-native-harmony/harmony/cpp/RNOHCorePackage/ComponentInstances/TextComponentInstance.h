#pragma once

#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/TextNode.h"

namespace rnoh {
    class TextComponentInstance : public CppComponentInstance {
    private:
        TextNode m_textNode;

    public:
        TextComponentInstance(Context context, facebook::react::Tag tag);

        void setProps(facebook::react::Props::Shared props) override;

        void setState(facebook::react::State::Shared state) override;

        TextNode &getLocalRootArkUINode() override;
    };
} // namespace rnoh