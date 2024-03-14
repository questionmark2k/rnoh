/**
 * Used only in C-API based Architecture.
 */
#pragma once
#include <react/renderer/scheduler/SchedulerDelegate.h>
#include <react/renderer/core/ComponentDescriptor.h>
#include <folly/dynamic.h>
#include "RNOH/TaskExecutor/TaskExecutor.h"
#include "RNOH/ShadowViewRegistry.h"
#include "RNOH/ComponentInstanceRegistry.h"
#include "RNOH/ComponentInstanceFactory.h"

namespace facebook {
    namespace react {
        class Scheduler;
    }
} // namespace facebook

namespace rnoh {

    class SchedulerDelegateCAPI : public facebook::react::SchedulerDelegate {
    public:
        SchedulerDelegateCAPI(TaskExecutor::Shared taskExecutor, ShadowViewRegistry::Shared shadowViewRegistry,
                              ComponentInstanceRegistry::Shared componentInstanceRegistry,
                              ComponentInstanceFactory::Shared componentInstanceFactory)
            : m_taskExecutor(taskExecutor), m_shadowViewRegistry(shadowViewRegistry),
              m_componentInstanceRegistry(std::move(componentInstanceRegistry)),
              m_componentInstanceFactory(std::move(componentInstanceFactory))
              {};


        ~SchedulerDelegateCAPI() { DLOG(INFO) << "~SchedulerDelegateCAPI"; }

        void schedulerDidFinishTransaction(facebook::react::MountingCoordinator::Shared mountingCoordinator) override {
            mountingCoordinator->getTelemetryController().pullTransaction(
                [this](facebook::react::MountingTransaction const &transaction,
                       facebook::react::SurfaceTelemetry const &surfaceTelemetry) {
                    // Will mount
                },
                [this](facebook::react::MountingTransaction const &transaction,
                       facebook::react::SurfaceTelemetry const &surfaceTelemetry) {

                },
                [this](facebook::react::MountingTransaction const &transaction,
                       facebook::react::SurfaceTelemetry const &surfaceTelemetry) {
                    // Did mount
                    m_taskExecutor->runTask(TaskThread::MAIN, [this, mutations = transaction.getMutations()] {
                        for (auto mutation : mutations) {
                            try {
                                this->handleMutation(mutation);
                            } catch (std::runtime_error &e) {
                                LOG(ERROR) << "Mutation " << this->getMutationNameFromType(mutation.type)
                                           << " failed: " << e.what();
                            }
                        }
                    });
                });
        }

        void schedulerDidRequestPreliminaryViewAllocation(facebook::react::SurfaceId surfaceId,
                                                          const facebook::react::ShadowNode &shadowNode) override {}

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

        void synchronouslyUpdateViewOnUIThread(facebook::react::Tag tag, folly::dynamic props,
                                               facebook::react::ComponentDescriptor const &componentDescriptor);

    private:
        TaskExecutor::Shared m_taskExecutor;
        ShadowViewRegistry::Shared m_shadowViewRegistry;
        ComponentInstanceRegistry::Shared m_componentInstanceRegistry;
        ComponentInstanceFactory::Shared m_componentInstanceFactory;
        facebook::react::ContextContainer::Shared m_contextContainer;

        void updateComponentWithShadowView(ComponentInstance::Shared const &componentInstance,
                                           facebook::react::ShadowView const &shadowView) {
            componentInstance->setLayout(shadowView.layoutMetrics);
            componentInstance->setProps(shadowView.props);
            componentInstance->setState(shadowView.state);
            componentInstance->setEventEmitter(shadowView.eventEmitter);
            componentInstance->finalizeUpdates();
        }

        void handleMutation(facebook::react::ShadowViewMutation mutation) {
            DLOG(INFO) << "mutation (type:" << this->getMutationNameFromType(mutation.type)
                       << "; new: " << mutation.newChildShadowView.tag
                       << " component type: " << mutation.newChildShadowView.componentName
                       << "; old: " << mutation.oldChildShadowView.tag << "; parent: " << mutation.parentShadowView.tag
                       << ")";
            switch (mutation.type) {
            case facebook::react::ShadowViewMutation::Create: {
                auto newChild = mutation.newChildShadowView;
                m_shadowViewRegistry->setShadowView(newChild.tag, newChild);
                auto componentInstance = m_componentInstanceFactory->create(newChild.tag, newChild.componentHandle, newChild.componentName);
                if (componentInstance != nullptr) {
                    updateComponentWithShadowView(componentInstance, newChild);
                    m_componentInstanceRegistry->insert(componentInstance);
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
                    parentComponentInstance->removeChild(
                        m_componentInstanceRegistry->findByTag(mutation.oldChildShadowView.tag));
                    parentComponentInstance->finalizeUpdates();
                }
                break;
            }
            case facebook::react::ShadowViewMutation::Update: {
                auto componentInstance = m_componentInstanceRegistry->findByTag(mutation.newChildShadowView.tag);
                if (componentInstance != nullptr) {
                    updateComponentWithShadowView(componentInstance, mutation.newChildShadowView);
                }
                m_shadowViewRegistry->setShadowView(mutation.newChildShadowView.tag, mutation.newChildShadowView);
                break;
            }
            }
        }

        std::string getMutationNameFromType(facebook::react::ShadowViewMutation::Type mutationType) {
            switch (mutationType) {
            case facebook::react::ShadowViewMutation::Create:
                return "CREATE";
            case facebook::react::ShadowViewMutation::Delete:
                return "DELETE";
            case facebook::react::ShadowViewMutation::Update:
                return "UPDATE";
            case facebook::react::ShadowViewMutation::Insert:
                return "INSERT";
            case facebook::react::ShadowViewMutation::Remove:
                return "REMOVE";
            case facebook::react::ShadowViewMutation::RemoveDeleteTree:
                return "REMOVE_DELETE_TREE";
            default:
                return "UNKNOWN";
            }
        }
    };

} // namespace rnoh
