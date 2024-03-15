#pragma once

#include "ArkUINode.h"

namespace rnoh {

class LoadingProgressNode : public ArkUINode {

public:
    LoadingProgressNode();

    void setLoadingProgressNodeColor(facebook::react::SharedColor const &color);

    void setLoadingProgressNodeanimating(const bool &enable);
};

} // namespace rnoh