#pragma once

#include <react/renderer/components/image/ImageEventEmitter.h>
#include <react/renderer/components/image/ImageShadowNode.h>
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/ImageNode.h"

namespace rnoh {
class ImageComponentInstance
    : public CppComponentInstance<facebook::react::ImageShadowNode>,
      public ImageNodeDelegate {
 private:
  ImageNode m_imageNode;
  struct ImageRawProps {
    std::optional<std::string> resizeMethod;
    std::optional<bool> focusable;
    std::optional<std::string> alt;
    static ImageRawProps getFromDynamic(folly::dynamic value);
  };
  ImageRawProps m_rawProps;

 public:
  ImageComponentInstance(Context context);
  void onPropsChanged(SharedConcreteProps const& props) override;
  void onStateChanged(SharedConcreteState const& state) override;

  void onComplete(float width, float height) override;
  void onError(int32_t errorCode) override;
  void onLoadStart();

  ImageNode& getLocalRootArkUINode() override;
};
} // namespace rnoh