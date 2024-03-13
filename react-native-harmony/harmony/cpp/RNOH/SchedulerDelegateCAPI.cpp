#include "SchedulerDelegateCAPI.h"
#include <react/renderer/scheduler/Scheduler.h>

namespace rnoh {

using namespace facebook;

void SchedulerDelegateCAPI::synchronouslyUpdateViewOnUIThread(
    facebook::react::Tag tag,
    folly::dynamic props,
    facebook::react::ComponentDescriptor const &componentDescriptor) {
        auto componentInstance = m_componentInstanceRegistry->findByTag(tag);
        if (componentInstance == nullptr) {
            return;
        }

        auto oldProps = componentInstance->getProps();
        auto newProps = componentDescriptor.cloneProps(facebook::react::PropsParserContext{1, *m_contextContainer}, oldProps, std::move(props));

        componentInstance->setProps(newProps);
        componentInstance->finalizeUpdates();
}

} // namespace rnoh
