#include "ImageNode.h"

#include <string_view>
#include "NativeNodeApi.h"

static constexpr ArkUI_NodeEventType IMAGE_NODE_EVENT_TYPES[] = {
    NODE_IMAGE_ON_COMPLETE,
    NODE_IMAGE_ON_ERROR};

namespace rnoh {

using namespace std::literals;
constexpr std::string_view ASSET_PREFIX = "asset://"sv;

ImageNode::ImageNode()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(
          ArkUI_NodeType::ARKUI_NODE_IMAGE)),
      m_childArkUINodeHandle(nullptr),
      m_imageNodeDelegate(nullptr) {
  for (auto eventType : IMAGE_NODE_EVENT_TYPES) {
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(
        m_nodeHandle, eventType, eventType, this));
  }
}

ImageNode::~ImageNode() {
  for (auto eventType : IMAGE_NODE_EVENT_TYPES) {
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, eventType);
  }
}

void ImageNode::setNodeDelegate(ImageNodeDelegate* imageNodeDelegate) {
  m_imageNodeDelegate = imageNodeDelegate;
}

void ImageNode::onNodeEvent(
    ArkUI_NodeEventType eventType,
    EventArgs& eventArgs) {
  if (eventType == ArkUI_NodeEventType::NODE_IMAGE_ON_COMPLETE) {
    if (m_imageNodeDelegate != nullptr && eventArgs[0].i32 == 1) {
      m_imageNodeDelegate->onComplete(eventArgs[1].f32, eventArgs[2].f32);
    }
  }

  if (eventType == ArkUI_NodeEventType::NODE_IMAGE_ON_ERROR) {
    if (m_imageNodeDelegate != nullptr) {
      m_imageNodeDelegate->onError(eventArgs[0].i32);
    }
  }
}

ImageNode& ImageNode::setSources(facebook::react::ImageSources const& src) {
  ArkUI_AttributeItem item;
  m_uri = src[0].uri;
  if (m_uri.rfind(ASSET_PREFIX, 0) == 0) {
    std::string resourceStr = std::string("resource://RAWFILE/") + "assets/";
    resourceStr += m_uri.substr(ASSET_PREFIX.size());
    item = {.string = resourceStr.c_str()};
  } else {
    item = {.string = m_uri.c_str()};
  }
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_IMAGE_SRC, &item));
  return *this;
}

ImageNode& ImageNode::setResizeMode(
    facebook::react::ImageResizeMode const& mode) {
  int32_t val = ARKUI_OBJECT_FIT_COVER;
  if (mode == facebook::react::ImageResizeMode::Cover) {
    val = ARKUI_OBJECT_FIT_COVER;
  } else if (mode == facebook::react::ImageResizeMode::Contain) {
    val = ARKUI_OBJECT_FIT_CONTAIN;
  } else if (mode == facebook::react::ImageResizeMode::Stretch) {
    val = ARKUI_OBJECT_FIT_FILL;
  } else if (
      mode == facebook::react::ImageResizeMode::Center ||
      mode == facebook::react::ImageResizeMode::Repeat) {
    val = ARKUI_OBJECT_FIT_NONE;
  }

  ArkUI_NumberValue value[] = {{.i32 = val}};
  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_IMAGE_OBJECT_FIT, &item));
  return *this;
}

ImageNode& ImageNode::setTintColor(
    facebook::react::SharedColor const& sharedColor) {
  if (!sharedColor) { // restore default value
    maybeThrow(NativeNodeApi::getInstance()->resetAttribute(
        m_nodeHandle, NODE_IMAGE_COLOR_FILTER));
    return *this;
  }

  facebook::react::ColorComponents com = colorComponentsFromColor(sharedColor);
  ArkUI_NumberValue value[] = {
      {.f32 = 0}, {.f32 = 0}, {.f32 = 0}, {.f32 = com.red},   {.f32 = 0},
      {.f32 = 0}, {.f32 = 0}, {.f32 = 0}, {.f32 = com.green}, {.f32 = 0},
      {.f32 = 0}, {.f32 = 0}, {.f32 = 0}, {.f32 = com.blue},  {.f32 = 0},
      {.f32 = 0}, {.f32 = 0}, {.f32 = 0}, {.f32 = 1},         {.f32 = 0}};

  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_IMAGE_COLOR_FILTER, &item));
  return *this;
}

ImageNode& ImageNode::setBlur(facebook::react::Float blur) {
  ArkUI_NumberValue value[] = {{.f32 = static_cast<float>(blur)}};
  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_BLUR, &item));
  return *this;
}

ImageNode& ImageNode::setObjectRepeat(
    facebook::react::ImageResizeMode const& resizeMode) {
  int32_t val = ARKUI_IMAGE_REPEAT_NONE;
  if (resizeMode == facebook::react::ImageResizeMode::Repeat) {
    val = ARKUI_IMAGE_REPEAT_XY;
  }

  ArkUI_NumberValue value[] = {{.i32 = val}};
  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_IMAGE_OBJECT_REPEAT, &item));
  return *this;
}

ImageNode& ImageNode::setInterpolation(int32_t interpolation) {
  ArkUI_NumberValue value[] = {{.i32 = interpolation}};
  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_IMAGE_INTERPOLATION, &item));
  return *this;
}

ImageNode& ImageNode::setDraggable(bool draggable) {
  ArkUI_NumberValue value[] = {{.i32 = static_cast<int32_t>(draggable)}};
  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_IMAGE_DRAGGABLE, &item));
  return *this;
}

ImageNode& ImageNode::setFocusable(bool focusable) {
  ArkUI_NumberValue value[] = {{.i32 = static_cast<int32_t>(focusable)}};
  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_FOCUSABLE, &item));
  return *this;
}

ImageNode& ImageNode::setResizeMethod(std::string const& resizeMethod) {
  auto autoResize = (resizeMethod != "scale") ? 1 : 0;
  ArkUI_NumberValue value[] = {{.i32 = autoResize}};
  ArkUI_AttributeItem item = {value, sizeof(value) / sizeof(ArkUI_NumberValue)};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_IMAGE_AUTO_RESIZE, &item));
  return *this;
}

ImageNode& ImageNode::setAlt(facebook::react::ImageSources const& src) {
  if (!src.empty()) {
    auto uri = src[0].uri;
    std::string resourceStr = std::string("resource://RAWFILE/") + "assets/";
    resourceStr += uri.substr(ASSET_PREFIX.size());
    ArkUI_AttributeItem item = {.string = resourceStr.c_str()};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(
        m_nodeHandle, NODE_IMAGE_ALT, &item));
  }
  return *this;
}

ImageNode& ImageNode::resetFocusable() {
  maybeThrow(NativeNodeApi::getInstance()->resetAttribute(
      m_nodeHandle, NODE_FOCUSABLE));
  return *this;
}
ImageNode& ImageNode::resetResizeMethod() {
  maybeThrow(NativeNodeApi::getInstance()->resetAttribute(
      m_nodeHandle, NODE_IMAGE_AUTO_RESIZE));
  return *this;
}

std::string ImageNode::getUri() {
  return m_uri;
}
} // namespace rnoh
