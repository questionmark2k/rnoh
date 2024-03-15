#include "ActivityIndicatorComponentInstance.h"

#include <sstream>
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/rncore/Props.h>
#include <glog/logging.h>

namespace rnoh {

ActivityIndicatorComponentInstance::ActivityIndicatorComponentInstance(Context context, facebook::react::Tag tag)
    : CppComponentInstance(std::move(context), tag) {}

void ActivityIndicatorComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
    auto old = std::dynamic_pointer_cast<const facebook::react::ActivityIndicatorViewProps>(m_props);
    CppComponentInstance::onPropsChanged(props);
    auto p = std::dynamic_pointer_cast<const facebook::react::ActivityIndicatorViewProps>(props);
    if (auto p = std::dynamic_pointer_cast<const facebook::react::ActivityIndicatorViewProps>(props)) {
        if (!old || p->animating != old->animating) {
            m_loadingProgressNode.setLoadingProgressNodeanimating(p->animating);
        }
        if (p->color && (!old || *(p->color) != *(old->color))) {
            m_loadingProgressNode.setLoadingProgressNodeColor(p->color);
        }
    }
    m_props = props;
}

LoadingProgressNode &ActivityIndicatorComponentInstance::getLocalRootArkUINode() {
    return m_loadingProgressNode;
}

} // namespace rnoh
