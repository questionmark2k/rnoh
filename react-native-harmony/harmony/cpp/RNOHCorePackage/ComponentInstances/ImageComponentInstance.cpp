#include "ImageComponentInstance.h"
#include <sstream>
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/image/ImageState.h>
#include <react/renderer/components/image/ImageProps.h>

namespace rnoh {

ImageComponentInstance::ImageComponentInstance(Context context) : CppComponentInstance(std::move(context)) {
    this->getLocalRootArkUINode().setNodeDelegate(this);
}

void ImageComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
    CppComponentInstance::onPropsChanged(props);
    if (!m_props || m_props->sources != props->sources) {
        this->getLocalRootArkUINode().setSources(props->sources);
    }

    if (!m_props || m_props->tintColor != props->tintColor) {
        this->getLocalRootArkUINode().SetTintColor(props->tintColor);
    }

    if (!m_props || m_props->blurRadius != props->blurRadius) {
        this->getLocalRootArkUINode().setBlur(props->blurRadius);
    }

    if (!m_props || m_props->resizeMode != props->resizeMode) {
        this->getLocalRootArkUINode().setObjectRepeat(props->resizeMode);
        this->getLocalRootArkUINode().setResizeMode(props->resizeMode);
    }

    if (m_isSetDefaultConfig == false) {
        this->getLocalRootArkUINode().setInterpolation(ARKUI_IMAGE_INTERPOLATION_HIGH);
        this->getLocalRootArkUINode().setDraggable(false);

        if (props->rawProps.count("resizeMethod") > 0) {
            this->getLocalRootArkUINode().setResizeMethod(props->rawProps["resizeMethod"].asString());
            m_resizeMethod = props->rawProps["resizeMethod"].asString();
        } else {
            this->getLocalRootArkUINode().setResizeMethod(m_resizeMethod);
        }

        if (props->rawProps.count("focusable") > 0 ) {
            this->getLocalRootArkUINode().setFocusable(props->rawProps["focusable"].asBool());
            m_focusable = props->rawProps["focusable"].asBool();
        } else {
            this->getLocalRootArkUINode().setFocusable(m_focusable);
        }

        if (props->rawProps.count("alt") > 0) {
            this->getLocalRootArkUINode().setAccessibilityText(props->rawProps["alt"].asString());
            m_alt = props->rawProps["alt"].asString();
        } else {
            this->getLocalRootArkUINode().setAccessibilityText(m_alt);
        }

        m_isSetDefaultConfig = true;
        return;
    }

    if (props->rawProps.count("resizeMethod") > 0 && props->rawProps["resizeMethod"].asString() != m_resizeMethod) {
        this->getLocalRootArkUINode().setResizeMethod(props->rawProps["resizeMethod"].asString());
        m_resizeMethod = props->rawProps["resizeMethod"].asString();
    }

    if (props->rawProps.count("focusable") > 0 && props->rawProps["focusable"].asBool() != m_focusable) {
        this->getLocalRootArkUINode().setFocusable(props->rawProps["focusable"].asBool());
        m_focusable = props->rawProps["focusable"].asBool();
    }

    if (props->rawProps.count("alt") > 0 && props->rawProps["alt"].asString() != m_alt) {
        this->getLocalRootArkUINode().setAccessibilityText(props->rawProps["alt"].asString());
        m_alt = props->rawProps["alt"].asString();
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
}
} // namespace rnoh