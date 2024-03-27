#pragma once
#include <react/renderer/graphics/Size.h>
#include <react/renderer/textlayoutmanager/TextLayoutManager.h>
#include <string>
#include "napi/native_api.h"
#include "RNOH/TaskExecutor/TaskExecutor.h"
#include "RNOH/FeatureFlagRegistry.h"

namespace rnoh {
class TextMeasurer : public facebook::react::TextLayoutManagerDelegate {

  public:
    TextMeasurer(napi_env env, napi_ref measureTextFnRef, std::shared_ptr<TaskExecutor> taskExecutor, FeatureFlagRegistry::Shared featureFlagManager)
        : m_env(env),
          m_measureTextFnRef(measureTextFnRef),
          m_taskExecutor(taskExecutor),
          m_featureFlagRegistry(featureFlagManager) {}

    facebook::react::TextMeasurement measure(facebook::react::AttributedString attributedString,
                                             facebook::react::ParagraphAttributes paragraphAttributes,
                                             facebook::react::LayoutConstraints layoutConstraints) override;

  private:
    napi_env m_env;
    napi_ref m_measureTextFnRef;
    std::shared_ptr<TaskExecutor> m_taskExecutor;
    FeatureFlagRegistry::Shared m_featureFlagRegistry;
};
} // namespace rnoh
