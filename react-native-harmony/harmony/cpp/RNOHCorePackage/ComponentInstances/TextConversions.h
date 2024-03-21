//
// Created on 2024/3/19.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef HARMONY_TEXTCONVERSIONS_H
#define HARMONY_TEXTCONVERSIONS_H

#include "react/renderer/attributedstring/primitives.h"
#include "react/renderer/components/text/ParagraphProps.h"
#include <arkui/native_type.h>

namespace rnoh {

    struct TextPaddingInfo {
        float top = 0.0;
        float bottom = 0.0;
        float left = 0.0;
        float right = 0.0;
    };

    class TextConversions {
    public:
        static int32_t getArkUIFontWeight(int32_t fontWeight);
        static int32_t getArkUITextCase(const facebook::react::TextTransform &textTransform);
        static int32_t getArkUIEllipsizeMode(const facebook::react::EllipsizeMode &ellipsizeMode);
        static int32_t getArkUITextAlignment(const facebook::react::TextAlignment &textAlign);
        static TextPaddingInfo getArkUIPadding(std::shared_ptr<const facebook::react::ParagraphProps> props);
        static std::optional<float> getMinFontSize(std::shared_ptr<const facebook::react::ParagraphProps> props);
        static std::optional<float> getMaxFontSize(std::shared_ptr<const facebook::react::ParagraphProps> props);
        static int32_t getStackAlign(const facebook::react::TextAlignment &alignHorizon,
                                     const std::string &alignVertical);

    private:
        static std::pair<std::optional<float>, std::optional<float>>
        calcHorizontalPadding(const YGStyle::Edges &yogaPadding, bool isRTL);
        static std::pair<std::optional<float>, std::optional<float>>
        calcVerticalPadding(YGStyle::Edges const &yogaPadding);
    };
}
#endif //HARMONY_TEXTCONVERSIONS_H