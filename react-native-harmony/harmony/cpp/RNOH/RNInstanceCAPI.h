#pragma once
#include <ace/xcomponent/native_interface_xcomponent.h>
#include "RNOH/RNInstance.h"
#include "RNOH/ComponentInstanceRegistry.h"
#include "RNOH/ComponentInstanceFactory.h"
#include "RNOH/arkui/XComponentSurface.h"

namespace rnoh {
    class RNInstanceCAPI : public RNInstance {
    private:
        std::unordered_map<facebook::react::SurfaceId, XComponentSurface> m_surfaceById;
        ComponentInstanceRegistry::Shared m_componentInstanceRegistry;
        ComponentInstanceFactory::Shared m_componentInstanceFactory;

    public:
        using RNInstance::RNInstance;

        void setCAPIArchDependencies(ComponentInstanceRegistry::Shared componentInstanceRegistry,
                                     ComponentInstanceFactory::Shared componentInstanceFactory);

        void registerNativeXComponentHandle(OH_NativeXComponent *nativeXComponent, facebook::react::Tag surfaceId);


        void createSurface(facebook::react::Tag surfaceId, std::string const &moduleName) override;
        void updateSurfaceConstraints(facebook::react::Tag surfaceId, float width, float height, float viewportOffsetX,
                                      float viewportOffsetY, float pixelRatio) override;
        void startSurface(facebook::react::Tag surfaceId, float width, float height, float viewportOffsetX,
                          float viewportOffsetY, float pixelRatio, folly::dynamic &&initialProps) override;
        void setSurfaceProps(facebook::react::Tag surfaceId, folly::dynamic &&props) override;
        void stopSurface(facebook::react::Tag surfaceId) override;
        void destroySurface(facebook::react::Tag surfaceId) override;
        void setSurfaceDisplayMode(facebook::react::Tag surfaceId, facebook::react::DisplayMode displayMode) override;
    };
} // namespace rnoh