#include "SchedulerDelegateCAPI.h"
#include <react/renderer/scheduler/Scheduler.h>

namespace rnoh {

using namespace facebook;

std::optional<react::SurfaceId> findSurfaceIdForComponentInstance(ComponentInstance::Shared componentInstance) {
    auto parentInstance = componentInstance->getParent().lock();
    while (parentInstance != nullptr) {
        componentInstance = std::move(parentInstance);
        parentInstance = componentInstance->getParent().lock();
    }
    auto rootComponentTag = componentInstance->getTag();

    // Fabric reserves numbers equal 1 mod 10 for SurfaceIds
    if (rootComponentTag % 10 == 1) {
        return static_cast<react::SurfaceId>(rootComponentTag);
    }

    return std::nullopt;
}

void SchedulerDelegateCAPI::synchronouslyUpdateViewOnUIThread(
    react::Tag tag,
    folly::dynamic props,
    react::ComponentDescriptor const &componentDescriptor) {
        auto componentInstance = m_componentInstanceRegistry->findByTag(tag);
        if (componentInstance == nullptr) {
            return;
        }

        auto surfaceId = findSurfaceIdForComponentInstance(componentInstance);
        if (!surfaceId.has_value()) {
            return;
        }

        auto oldProps = componentInstance->getProps();
        auto newProps = componentDescriptor.cloneProps(react::PropsParserContext{surfaceId.value(), *m_contextContainer}, oldProps, std::move(props));

        componentInstance->setProps(newProps);
        componentInstance->finalizeUpdates();
}

} // namespace rnoh
