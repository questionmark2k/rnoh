#include "ImageComponentInstance.h"
#include <sstream>
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/image/ImageState.h>
#include <react/renderer/components/image/ImageProps.h>

namespace rnoh {

ImageComponentInstance::ImageComponentInstance(Context context) : CppComponentInstance(std::move(context)) {
    this->getLocalRootArkUINode().setNodeDelegate(this);
    this->getLocalRootArkUINode().setInterpolation(ARKUI_IMAGE_INTERPOLATION_HIGH);
    this->getLocalRootArkUINode().setDraggable(false);
}

void ImageComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
    CppComponentInstance::onPropsChanged(props);

    auto rawProps = ImageRawProps::getFromDynamic(props->rawProps);

    if (!m_props || m_props->sources != props->sources) {
        this->getLocalRootArkUINode().setSources(props->sources);
        if (!this->getLocalRootArkUINode().getUri().empty()) {
            onLoadStart();
        }
    }

    if (!m_props || m_props->tintColor != props->tintColor) {
        this->getLocalRootArkUINode().setTintColor(props->tintColor);
    }

    if (!m_props || m_props->blurRadius != props->blurRadius) {
        this->getLocalRootArkUINode().setBlur(props->blurRadius);
    }

    if (!m_props || m_props->resizeMode != props->resizeMode) {
        this->getLocalRootArkUINode().setObjectRepeat(props->resizeMode);
        this->getLocalRootArkUINode().setResizeMode(props->resizeMode);
    }

    if (!m_props || m_props->defaultSources != props->defaultSources) {
        this->getLocalRootArkUINode().setAlt(props->defaultSources);
    }

    if (m_rawProps.resizeMethod != rawProps.resizeMethod) {
        m_rawProps.resizeMethod = rawProps.resizeMethod;
        if (m_rawProps.resizeMethod.has_value()) {
            this->getLocalRootArkUINode().setResizeMethod(m_rawProps.resizeMethod.value());
        } else {
            this->getLocalRootArkUINode().resetResizeMethod();
        }
    }

    if (m_rawProps.focusable != rawProps.focusable) {
        m_rawProps.focusable = rawProps.focusable;
        if (m_rawProps.focusable.has_value()) {
            this->getLocalRootArkUINode().setFocusable(m_rawProps.focusable.value());
        } else {
            this->getLocalRootArkUINode().resetFocusable();
        }
    }

    if (m_rawProps.alt != rawProps.alt) {
        m_rawProps.alt = rawProps.alt;
        if (m_rawProps.alt.has_value()) {
            this->getLocalRootArkUINode().setAccessibilityText(m_rawProps.alt.value());
        } else {
            this->getLocalRootArkUINode().resetAccessibilityText();
        }
    }
}

void ImageComponentInstance::onStateChanged(SharedConcreteState const &state) {
    CppComponentInstance::onStateChanged(state);
    auto vector = {state->getData().getImageSource()};
    this->getLocalRootArkUINode().setSources(vector);
    this->getLocalRootArkUINode().setBlur(state->getData().getBlurRadius());
}

ImageNode &ImageComponentInstance::getLocalRootArkUINode() {
    return m_imageNode;
}

void ImageComponentInstance::onComplete(float width, float height) {
    if (m_eventEmitter == nullptr) {
        return;
    }

    std::string uri = this->getLocalRootArkUINode().getUri();
    m_eventEmitter->dispatchEvent("load", [=](facebook::jsi::Runtime &runtime) {
        auto payload = facebook::jsi::Object(runtime);
        auto source = facebook::jsi::Object(runtime);
        source.setProperty(runtime, "width", width);
        source.setProperty(runtime, "height", height);
        source.setProperty(runtime, "uri", uri.c_str());
        payload.setProperty(runtime, "source", source);
        return payload;
    });
    m_eventEmitter->onLoadEnd();
}

void ImageComponentInstance::onError(int32_t errorCode) {
    if (m_eventEmitter == nullptr) {
        return;
    }

    std::string errMsg = "error message: the image load failed, ";
    if (errorCode == 401) {
        errMsg = "error message: the image could not be obtained because the image path is invalid, ";
    } else if (errorCode == 103101) {
        errMsg = "error message: the image format is not supported, ";
    }

    errMsg += std::string("error code: ") + std::to_string(errorCode);
    m_eventEmitter->dispatchEvent("error", [errMsg](facebook::jsi::Runtime &runtime) {
        auto payload = facebook::jsi::Object(runtime);
        auto source = facebook::jsi::Object(runtime);
        source.setProperty(runtime, "error", errMsg);
        payload.setProperty(runtime, "source", source);
        return payload;
    });
    m_eventEmitter->onLoadEnd();
}

void ImageComponentInstance::onLoadStart() {
    if (m_eventEmitter) {
        m_eventEmitter->onLoadStart();
    }
}

ImageComponentInstance::ImageRawProps ImageComponentInstance::ImageRawProps::getFromDynamic(folly::dynamic value) {
    auto resizeMethod = (value.count("resizeMethod") > 0) ? std::optional(value["resizeMethod"].asString()) : std::nullopt;
    auto focusable = (value.count("focusable") > 0) ? std::optional(value["focusable"].asBool()) : std::nullopt;
    auto alt = (value.count("alt") > 0) ? std::optional(value["alt"].asString()) : std::nullopt;
    return {resizeMethod, focusable, alt};
}

} // namespace rnoh