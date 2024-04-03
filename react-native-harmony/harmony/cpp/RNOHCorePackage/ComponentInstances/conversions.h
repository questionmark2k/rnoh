#pragma once
#include <arkui/native_type.h>
#include "react/renderer/components/view/primitives.h"

namespace rnoh {
ArkUI_ScrollSnapAlign getArkUI_ScrollSnapAlign(
    facebook::react::ScrollViewSnapToAlignment snapToAlignment) {
  if (snapToAlignment == facebook::react::ScrollViewSnapToAlignment::Start) {
    return ArkUI_ScrollSnapAlign::ARKUI_SCROLL_SNAP_ALIGN_START;
  } else if (
      snapToAlignment == facebook::react::ScrollViewSnapToAlignment::Center) {
    return ArkUI_ScrollSnapAlign::ARKUI_SCROLL_SNAP_ALIGN_CENTER;
  } else if (
      snapToAlignment == facebook::react::ScrollViewSnapToAlignment::End) {
    return ArkUI_ScrollSnapAlign::ARKUI_SCROLL_SNAP_ALIGN_END;
  } else {
    return ArkUI_ScrollSnapAlign::ARKUI_SCROLL_SNAP_ALIGN_NONE;
  }
}
} // namespace rnoh