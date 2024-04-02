#pragma once

#include "react/renderer/attributedstring/primitives.h"
#include "react/renderer/components/textinput/primitives.h"
#include "react/renderer/components/view/primitives.h"
#include <arkui/native_type.h>
#include <bits/alltypes.h>
#include <glog/logging.h>

namespace rnoh {
    inline ArkUI_BorderStyle convertReactBorderStyleToArk(facebook::react::BorderStyle rnBorder) {
        switch (rnBorder) {
        case facebook::react::BorderStyle::Solid:
            return ArkUI_BorderStyle::ARKUI_BORDER_STYLE_SOLID;
        case facebook::react::BorderStyle::Dotted:
            return ArkUI_BorderStyle::ARKUI_BORDER_STYLE_DOTTED;
        case facebook::react::BorderStyle::Dashed:
            return ArkUI_BorderStyle::ARKUI_BORDER_STYLE_DASHED;
        default:
            LOG(WARNING) << "Invalid BorderStyle";
            return ArkUI_BorderStyle::ARKUI_BORDER_STYLE_SOLID;
        }
    }

    inline ArkUI_TextAreaType convertTextAreaInputType(facebook::react::KeyboardType keyboardType) {
        switch (keyboardType) {
        case facebook::react::KeyboardType::Numeric:
        case facebook::react::KeyboardType::NumberPad:
            return ARKUI_TEXTAREA_TYPE_NUMBER;
        case facebook::react::KeyboardType::PhonePad:
            return ARKUI_TEXTAREA_TYPE_PHONE_NUMBER;
        case facebook::react::KeyboardType::EmailAddress:
            return ARKUI_TEXTAREA_TYPE_EMAIL;
        default:
            return ARKUI_TEXTAREA_TYPE_NORMAL;
        }
    }

    inline uint32_t convertColorToTranslucent(facebook::react::SharedColor const &color) {
        auto colorComponents = colorComponentsFromColor(color);
        uint32_t red = static_cast<uint32_t>(colorComponents.red * 255) & 0xFF;
        uint32_t green = static_cast<uint32_t>(colorComponents.green * 255) & 0xFF;
        uint32_t blue = static_cast<uint32_t>(colorComponents.blue * 255) & 0xFF;
        uint32_t alpha = 0x55;
        auto colorValue = (alpha << 24) | (red << 16) | (green << 8) | blue;
        return colorValue;
    }

    inline ArkUI_TextInputType convertInputType(facebook::react::KeyboardType keyboardType) {
        switch (keyboardType) {
        case facebook::react::KeyboardType::Numeric:
        case facebook::react::KeyboardType::NumberPad:
            return ARKUI_TEXTINPUT_TYPE_NUMBER;
        case facebook::react::KeyboardType::DecimalPad:
            return ARKUI_TEXTINPUT_TYPE_NUMBER_DECIMAL;
        case facebook::react::KeyboardType::PhonePad:
            return ARKUI_TEXTINPUT_TYPE_PHONE_NUMBER;
        case facebook::react::KeyboardType::EmailAddress:
            return ARKUI_TEXTINPUT_TYPE_EMAIL;
        default:
            return ARKUI_TEXTINPUT_TYPE_NORMAL;
        }
    }

    inline ArkUI_EnterKeyType convertEnterKeyType(facebook::react::ReturnKeyType returnKeyType) {
        switch (returnKeyType) {
        case facebook::react::ReturnKeyType::Done:
            return ARKUI_ENTER_KEY_TYPE_DONE;
        case facebook::react::ReturnKeyType::Go:
            return ARKUI_ENTER_KEY_TYPE_GO;
        case facebook::react::ReturnKeyType::Next:
            return ARKUI_ENTER_KEY_TYPE_NEXT;
        case facebook::react::ReturnKeyType::Search:
            return ARKUI_ENTER_KEY_TYPE_SEARCH;
        case facebook::react::ReturnKeyType::Send:
            return ARKUI_ENTER_KEY_TYPE_SEND;
        default:
            return ARKUI_ENTER_KEY_TYPE_DONE;
        }
    }

    inline ArkUI_NumberValue convertTextAlign(facebook::react::TextAlignment alignment) {
        switch (alignment) {
        case facebook::react::TextAlignment::Natural:
        case facebook::react::TextAlignment::Left:
            return {.i32 = static_cast<int32_t>(ARKUI_TEXT_ALIGNMENT_START)};
        case facebook::react::TextAlignment::Right:
            return {.i32 = static_cast<int32_t>(ARKUI_TEXT_ALIGNMENT_END)};
        case facebook::react::TextAlignment::Center:
            return {.i32 = static_cast<int32_t>(ARKUI_TEXT_ALIGNMENT_CENTER)};
        case facebook::react::TextAlignment::Justified:
            return {.i32 = static_cast<int32_t>(ARKUI_TEXT_ALIGNMENT_JUSTIFY)};
        default:
            return {.i32 = static_cast<int32_t>(ARKUI_TEXT_ALIGNMENT_START)};
        }
    }

    inline ArkUI_NumberValue convertFontWeight(int fontWeight) {
        if (fontWeight < 200) {
            return {.i32 = ARKUI_FONT_WEIGHT_W100};
        }
        if (fontWeight < 300) {
            return {.i32 = ARKUI_FONT_WEIGHT_W200};
        }
        if (fontWeight < 400) {
            return {.i32 = ARKUI_FONT_WEIGHT_W300};
        }
        if (fontWeight < 500) {
            return {.i32 = ARKUI_FONT_WEIGHT_W400};
        }
        if (fontWeight < 600) {
            return {.i32 = ARKUI_FONT_WEIGHT_W500};
        }
        if (fontWeight < 700) {
            return {.i32 = ARKUI_FONT_WEIGHT_W600};
        }
        if (fontWeight < 800) {
            return {.i32 = ARKUI_FONT_WEIGHT_W700};
        }
        if (fontWeight < 900) {
            return {.i32 = ARKUI_FONT_WEIGHT_W800};
        }
        return {.i32 = ARKUI_FONT_WEIGHT_W900};
    }
} // namespace rnoh
