#pragma once
#include <ace/xcomponent/native_interface_xcomponent.h>
#include "RNOH/RNInstance.h"
#include "RNOH/ComponentInstanceRegistry.h"
#include "RNOH/ComponentInstanceFactory.h"

namespace rnoh {
    class RNInstanceCAPI : public RNInstance {
    private:
        std::unordered_map<facebook::react::Tag, OH_NativeXComponent *> m_nativeXComponentBySurfaceId;
        ComponentInstanceRegistry::Shared m_componentInstanceRegistry;
        ComponentInstanceFactory::Shared m_componentInstanceFactory;

    public:
        using RNInstance::RNInstance;

        void setCAPIArchDependencies(ComponentInstanceRegistry::Shared componentInstanceRegistry,
                                     ComponentInstanceFactory::Shared componentInstanceFactory) {
            m_componentInstanceRegistry = componentInstanceRegistry;
            m_componentInstanceFactory = componentInstanceFactory;
        }

        void registerNativeXComponentHandle(OH_NativeXComponent *nativeXComponent, facebook::react::Tag surfaceId) {
            if (nativeXComponent == nullptr) {
                return;
            }
            if (m_nativeXComponentBySurfaceId.find(surfaceId) == m_nativeXComponentBySurfaceId.end()) {
                m_nativeXComponentBySurfaceId[surfaceId] = nativeXComponent;
                return;
            }
            if (m_nativeXComponentBySurfaceId[surfaceId] != nativeXComponent) {
                OH_NativeXComponent *tmp = m_nativeXComponentBySurfaceId[surfaceId];
                delete tmp;
                tmp = nullptr;
                m_nativeXComponentBySurfaceId[surfaceId] = nativeXComponent;
            }
        }


        void startSurface(facebook::react::Tag surfaceId, float width, float height, float viewportOffsetX,
                          float viewportOffsetY, float pixelRatio, folly::dynamic &&initialProps) override {
            if (m_nativeXComponentBySurfaceId.find(surfaceId) == m_nativeXComponentBySurfaceId.end()) {
                LOG(ERROR) << "Couldn't find XComponent for surfaceId: " << surfaceId;
                return;
            }
            auto rootInstanceComponent =
                m_componentInstanceFactory->create({.componentName = "RootView", .tag = surfaceId});
            if (rootInstanceComponent == nullptr) {
                LOG(ERROR) << "Couldn't create rootInstanceComponent for surface with id: " << surfaceId;
                return;
            }

            m_componentInstanceRegistry->insert(rootInstanceComponent);
            rootInstanceComponent->getLocalRootArkUINode().setSize({.width = 512, .height = 512});
            rootInstanceComponent->finalizeUpdates();
            OH_NativeXComponent_AttachNativeRootNode(
                m_nativeXComponentBySurfaceId.at(surfaceId),
                rootInstanceComponent->getLocalRootArkUINode().getArkUINodeHandle());
            RNInstance::startSurface(surfaceId, width, height, viewportOffsetX, viewportOffsetY, pixelRatio,
                                     std::move(initialProps));
        }
    };
} // namespace rnoh