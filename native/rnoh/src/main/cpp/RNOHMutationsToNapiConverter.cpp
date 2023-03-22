#include "RNOHMutationsToNapiConverter.h"
#include <react/renderer/components/view/ViewProps.h>

using namespace facebook::react;

RNOHMutationsToNapiConverter::RNOHMutationsToNapiConverter(ArkJS arkJs) : m_arkJs(arkJs) {
}

napi_value RNOHMutationsToNapiConverter::convert(ShadowViewMutationList const &mutations) {
    std::vector<napi_value> napiMutations;
    for (auto &mutation : mutations) {
        auto objBuilder = m_arkJs.createObjectBuilder().addProperty("type", mutation.type);
        switch (mutation.type) {
        case ShadowViewMutation::Type::Create: {
            objBuilder
                .addProperty("descriptor", this->convertShadowView(mutation.newChildShadowView));
            break;
        }
        case ShadowViewMutation::Type::Remove: {
            objBuilder
                .addProperty("childTag", mutation.oldChildShadowView.tag)
                .addProperty("parentTag", mutation.parentShadowView.tag);
            break;
        }
        case ShadowViewMutation::Type::Update: {
            objBuilder
                .addProperty("descriptor", this->convertShadowView(mutation.newChildShadowView));
            break;
        }
        case ShadowViewMutation::Type::Insert: {
            objBuilder
                .addProperty("childTag", mutation.newChildShadowView.tag)
                .addProperty("parentTag", mutation.parentShadowView.tag);
            break;
        }
        case ShadowViewMutation::Type::Delete: {
            objBuilder.addProperty("tag", mutation.oldChildShadowView.tag);
            break;
        }
        }
        napiMutations.push_back(objBuilder.build());
    }
    return m_arkJs.createArray(napiMutations);
}

napi_value RNOHMutationsToNapiConverter::convertShadowView(ShadowView const shadowView) {
    auto propsObjBuilder = m_arkJs.createObjectBuilder();
    propsObjBuilder
        .addProperty("top", shadowView.layoutMetrics.frame.origin.y)
        .addProperty("left", shadowView.layoutMetrics.frame.origin.x)
        .addProperty("width", shadowView.layoutMetrics.frame.size.width)
        .addProperty("height", shadowView.layoutMetrics.frame.size.height);
    if (auto viewProps = std::dynamic_pointer_cast<const ViewProps>(shadowView.props)) {
        propsObjBuilder.addProperty("backgroundColor", viewProps->backgroundColor);
    }
    return m_arkJs.createObjectBuilder()
        .addProperty("tag", shadowView.tag)
        .addProperty("type", shadowView.componentName)
        .addProperty("props", propsObjBuilder.build())
        .build();
}