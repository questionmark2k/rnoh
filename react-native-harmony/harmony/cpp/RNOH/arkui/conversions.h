#include "react/renderer/components/view/primitives.h"
#include <arkui/native_type.h>
#include <glog/logging.h>

namespace rnoh {
    ArkUI_BorderStyle convertReactBorderStyleToArk(facebook::react::BorderStyle rnBorder) {
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
} // namespace rnoh
