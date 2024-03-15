#pragma once

#include "ArkUINode.h"

namespace rnoh {

class LoadingProgressNode : public ArkUINode {

public:
    LoadingProgressNode();

    void setLoadingProgressNodeColor(facebook::react::SharedColor const &color);

    void setLoadingProgressNodeAnimating(const bool &enable);
};

} // namespace rnoh