#pragma once

#include "ArkUINode.h"

namespace rnoh {

    class StackNode : public ArkUINode {
    public:
        StackNode();

        void insertChild(ArkUINode &child, std::size_t index);
        void removeChild(ArkUINode &child);
    };

} // namespace rnoh