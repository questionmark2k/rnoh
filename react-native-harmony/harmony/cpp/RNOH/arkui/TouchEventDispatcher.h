#pragma once

#include <unordered_map>
#include <react/renderer/graphics/Point.h>
#include <arkui/native_event.h>
#include "RNOH/TouchTarget.h"

namespace rnoh {
    class TouchEventDispatcher {
        public:
            using TouchId = int;

            void dispatchTouchEvent(ArkUI_NodeTouchEvent event, TouchTarget::Shared const &rootTarget);
        private:
            void registerTargetForTouch(ArkUI_NodeTouchPoint touchPoint, TouchTarget::Shared const &rootTarget);

            std::unordered_map<TouchId, TouchTarget::Weak> m_touchTargetTagById;
    };
}