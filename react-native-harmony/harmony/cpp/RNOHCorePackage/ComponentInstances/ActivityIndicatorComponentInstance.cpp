#include "ActivityIndicatorComponentInstance.h"

#include <sstream>
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/rncore/Props.h>
#include <glog/logging.h>

namespace rnoh {

ActivityIndicatorComponentInstance::ActivityIndicatorComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {}

void ActivityIndicatorComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
    auto old = m_props;
    CppComponentInstance::onPropsChanged(props);
    if (!old || props->animating != old->animating) {
        m_loadingProgressNode.setLoadingProgressNodeAnimating(props->animating);
    }
    if (props->color && (!old || *(props->color) != *(old->color))) {
        m_loadingProgressNode.setLoadingProgressNodeColor(props->color);
    }
}

LoadingProgressNode &ActivityIndicatorComponentInstance::getLocalRootArkUINode() {
    return m_loadingProgressNode;
}

} // namespace rnoh
