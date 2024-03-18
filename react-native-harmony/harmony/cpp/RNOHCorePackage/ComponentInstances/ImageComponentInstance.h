#pragma once

#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/ImageNode.h"
#include <react/renderer/components/image/ImageEventEmitter.h>
#include <react/renderer/components/image/ImageShadowNode.h>

namespace rnoh {
class ImageComponentInstance : public CppComponentInstance<facebook::react::ImageShadowNode>, public ImageNodeDelegate {
private:
    ImageNode m_imageNode;
    bool m_isSetDefaultConfig = false;
    std::string m_resizeMethod = "auto";
    bool m_focusable = false;
    std::string m_alt;
public:
    ImageComponentInstance(Context context);
    void onPropsChanged(SharedConcreteProps const &props) override;
    void onStateChanged(SharedConcreteState const &state) override;

    void onComplete(float width, float height) override;
    void onError(int32_t errorCode) override;

    ImageNode &getLocalRootArkUINode() override;
};
} // namespace rnoh