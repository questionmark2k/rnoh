#include "RNInstanceCAPI.h"

namespace rnoh {
void RNInstanceCAPI::setCAPIArchDependencies(ComponentInstanceRegistry::Shared componentInstanceRegistry, ComponentInstanceFactory::Shared componentInstanceFactory) {
    m_componentInstanceRegistry = componentInstanceRegistry;
    m_componentInstanceFactory = componentInstanceFactory;
}

void RNInstanceCAPI::registerNativeXComponentHandle(OH_NativeXComponent *nativeXComponent, facebook::react::Tag surfaceId) {
    if (nativeXComponent == nullptr) {
        return;
    }
    auto it = m_surfaceById.find(surfaceId);
    if (it == m_surfaceById.end()) {
        LOG(ERROR) << "Surface with id: " << surfaceId << " not found";
        return;
    }
    it->second.attachNativeXComponent(nativeXComponent);
}

void RNInstanceCAPI::createSurface(facebook::react::Tag surfaceId, std::string const &moduleName) {
    m_surfaceById.emplace(surfaceId, XComponentSurface(scheduler, m_componentInstanceRegistry, m_componentInstanceFactory, surfaceId, moduleName));
}

void RNInstanceCAPI::updateSurfaceConstraints(facebook::react::Tag surfaceId, float width, float height, float viewportOffsetX, float viewportOffsetY, float pixelRatio) {
    auto it = m_surfaceById.find(surfaceId);
    if (it == m_surfaceById.end()) {
        return;
    }
    it->second.updateConstraints(width, height, viewportOffsetX, viewportOffsetY, pixelRatio);
}

void RNInstanceCAPI::startSurface(
    facebook::react::Tag surfaceId, 
    float width, float height, 
    float viewportOffsetX, float viewportOffsetY, 
    float pixelRatio, 
    folly::dynamic &&initialProps
) {
    auto it = m_surfaceById.find(surfaceId);
    if (it == m_surfaceById.end()) {
        return;
    }
    it->second.start(width, height, viewportOffsetX, viewportOffsetY, pixelRatio, std::move(initialProps));
}

void RNInstanceCAPI::setSurfaceProps(facebook::react::Tag surfaceId, folly::dynamic &&props) {
    auto it = m_surfaceById.find(surfaceId);
    if (it == m_surfaceById.end()) {
        return;
    }
    it->second.setProps(std::move(props));
}

void RNInstanceCAPI::stopSurface(facebook::react::Tag surfaceId) {
    auto it = m_surfaceById.find(surfaceId);
    if (it == m_surfaceById.end()) {
        return;
    }
    it->second.stop();
}

void RNInstanceCAPI::destroySurface(facebook::react::Tag surfaceId) {
    auto it = m_surfaceById.find(surfaceId);
    if (it == m_surfaceById.end()) {
        return;
    }
    m_surfaceById.erase(it);
}

void RNInstanceCAPI::setSurfaceDisplayMode(facebook::react::Tag surfaceId, facebook::react::DisplayMode displayMode) {
    auto it = m_surfaceById.find(surfaceId);
    if (it == m_surfaceById.end()) {
        return;
    }
    it->second.setDisplayMode(displayMode);
}

} // namespace rnoh