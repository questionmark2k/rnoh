/**
 * Used only in C-API based Architecture.
 */
#pragma once
#include <react/renderer/scheduler/SchedulerDelegate.h>
#include <folly/dynamic.h>
#include "RNOH/TaskExecutor/TaskExecutor.h"
#include "RNOH/ShadowViewRegistry.h"
#include "RNOH/ComponentInstanceRegistry.h"
#include "RNOH/ComponentInstanceFactory.h"

namespace rnoh {

    class SchedulerDelegateCAPI : public facebook::react::SchedulerDelegate {
    public:
        SchedulerDelegateCAPI(TaskExecutor::Shared taskExecutor, ShadowViewRegistry::Shared shadowViewRegistry,
                              ComponentInstanceRegistry::Shared componentInstanceRegistry,
                              ComponentInstanceFactory::Shared componentInstanceFactory)
            : m_taskExecutor(taskExecutor), m_shadowViewRegistry(shadowViewRegistry),
              m_componentInstanceRegistry(componentInstanceRegistry),
              m_componentInstanceFactory(std::move(componentInstanceFactory)){};


        ~SchedulerDelegateCAPI() = default;

        void schedulerDidFinishTransaction(facebook::react::MountingCoordinator::Shared mountingCoordinator) override {
            mountingCoordinator->getTelemetryController().pullTransaction(
                [this](facebook::react::MountingTransaction const &transaction,
                       facebook::react::SurfaceTelemetry const &surfaceTelemetry) {
                    // Will mount
                },
                [this](facebook::react::MountingTransaction const &transaction,
                       facebook::react::SurfaceTelemetry const &surfaceTelemetry) {
                    // Mounting
                },
                [this](facebook::react::MountingTransaction const &transaction,
                       facebook::react::SurfaceTelemetry const &surfaceTelemetry) {
                    // Did mount
                    m_taskExecutor->runTask(TaskThread::MAIN, [this, mutations = transaction.getMutations()] {
                        for (auto mutation : mutations) {
                            this->handleMutation(mutation);
                        }
                    });
                });
        }

        void schedulerDidRequestPreliminaryViewAllocation(facebook::react::SurfaceId surfaceId,
                                                          const facebook::react::ShadowNode &shadowNode) override {
        }

        void schedulerDidDispatchCommand(const facebook::react::ShadowView &shadowView, std::string const &commandName,
                                         folly::dynamic const &args) override {
            auto componentInstance = m_componentInstanceRegistry->findByTag(shadowView.tag);
            if (componentInstance != nullptr) {
                componentInstance->handleCommand(commandName, args);
            }
        }

        void schedulerDidSendAccessibilityEvent(const facebook::react::ShadowView &shadowView,
                                                std::string const &eventType) override {}

        void schedulerDidSetIsJSResponder(facebook::react::ShadowView const &shadowView, bool isJSResponder,
                                          bool blockNativeResponder) override {}

    private:
        TaskExecutor::Shared m_taskExecutor;
        ShadowViewRegistry::Shared m_shadowViewRegistry;
        ComponentInstanceRegistry::Shared m_componentInstanceRegistry;
        ComponentInstanceFactory::Shared m_componentInstanceFactory;

        void handleMutation(facebook::react::ShadowViewMutation mutation) {
            switch (mutation.type) {
            case facebook::react::ShadowViewMutation::Create: {
                auto newChild = mutation.newChildShadowView;
                m_shadowViewRegistry->setShadowView(newChild.tag, newChild);
                auto componentInstance = m_componentInstanceFactory->create(
                    {.tag = newChild.tag, .componentName = mutation.newChildShadowView.componentName});
                if (componentInstance != nullptr) {
                    componentInstance->setProps(mutation.newChildShadowView.props);
                    componentInstance->setState(mutation.newChildShadowView.state);
                    componentInstance->setLayout(mutation.newChildShadowView.layoutMetrics);
                    m_componentInstanceRegistry->insert(componentInstance);
                    componentInstance->finalizeUpdates();
                }
                break;
            }
            case facebook::react::ShadowViewMutation::Delete: {
                auto oldChild = mutation.oldChildShadowView;
                m_shadowViewRegistry->clearShadowView(oldChild.tag);
                m_componentInstanceRegistry->deleteByTag(oldChild.tag);
                break;
            }
            case facebook::react::ShadowViewMutation::Insert: {
                auto parentComponentInstance = m_componentInstanceRegistry->findByTag(mutation.parentShadowView.tag);
                auto newChildComponentInstance =
                    m_componentInstanceRegistry->findByTag(mutation.newChildShadowView.tag);
                if (parentComponentInstance != nullptr && newChildComponentInstance != nullptr) {
                    parentComponentInstance->insertChild(newChildComponentInstance, mutation.index);
                    parentComponentInstance->finalizeUpdates();
                    newChildComponentInstance->finalizeUpdates();
                }
                break;
            }
            case facebook::react::ShadowViewMutation::Remove: {
                auto parentComponentInstance = m_componentInstanceRegistry->findByTag(mutation.parentShadowView.tag);
                if (parentComponentInstance) {
                    parentComponentInstance->removeChild(m_componentInstanceRegistry->findByTag(mutation.oldChildShadowView.tag));
                    parentComponentInstance->finalizeUpdates();
                }
                break;
            }
            case facebook::react::ShadowViewMutation::Update: {
                auto componentInstance = m_componentInstanceRegistry->findByTag(mutation.newChildShadowView.tag);
                if (componentInstance != nullptr) {
                    componentInstance->setProps(mutation.newChildShadowView.props);
                    componentInstance->setState(mutation.newChildShadowView.state);
                    componentInstance->setLayout(mutation.newChildShadowView.layoutMetrics);
                    componentInstance->finalizeUpdates();
                }
                m_shadowViewRegistry->setShadowView(mutation.newChildShadowView.tag, mutation.newChildShadowView);
                break;
            }
            }
        }
    };

} // namespace rnoh
