#include "HarmonyReactMarker.h"

namespace rnoh {
void HarmonyReactMarker::setLogPerfMarkerIfNeeded() {
  static std::once_flag flag{};
  std::call_once(flag, []() {
    ReactMarker::logTaggedMarkerImpl = HarmonyReactMarker::logPerfMarker;
    ReactMarker::logTaggedMarkerBridgelessImpl =
        HarmonyReactMarker::logPerfMarker;
    ReactMarker::getAppStartTimeImpl = HarmonyReactMarker::getAppStartTime;
  });
}

std::string makeMessage(const std::string& marker, const std::string& tag) {
  std::string message = "ReactMarker::" + marker;
  if (!tag.empty()) {
    message += "::Tag::";
    message += tag;
  }
  return message;
};

int32_t getMessageId(const std::string& message) {
  std::hash<std::string> hasher;
  size_t hash = hasher(message);
  // Ensure the hash fits into int32_t. We don't care that much about collisions
  int32_t small_hash = hash ^ (hash >> 32);
  return small_hash;
};

void HarmonyReactMarker::logMarker(const std::string& marker) {
  OH_HiTrace_StartTrace(makeMessage(marker, "").c_str());
  OH_HiTrace_FinishTrace();
}

void HarmonyReactMarker::logMarkerStart(
    const std::string& marker,
    const std::string& tag) {
  auto message = makeMessage(marker, tag).c_str();
  OH_HiTrace_StartAsyncTrace(message, getMessageId(message));
}

void HarmonyReactMarker::logMarkerFinish(
    const std::string& marker,
    const std::string& tag) {
  auto message = makeMessage(marker, tag).c_str();
  OH_HiTrace_FinishAsyncTrace(message, getMessageId(message));
}

void HarmonyReactMarker::logPerfMarker(
    const ReactMarker::ReactMarkerId markerId,
    const char* tag) {
  switch (markerId) {
    case ReactMarker::RUN_JS_BUNDLE_START:
      HarmonyReactMarker::logMarkerStart("RUN_JS_BUNDLE", tag);
      break;
    case ReactMarker::RUN_JS_BUNDLE_STOP:
      HarmonyReactMarker::logMarkerFinish("RUN_JS_BUNDLE", tag);
      break;
    case ReactMarker::CREATE_REACT_CONTEXT_STOP:
      HarmonyReactMarker::logMarker("CREATE_REACT_CONTEXT_END");
      break;
    case ReactMarker::JS_BUNDLE_STRING_CONVERT_START:
      HarmonyReactMarker::logMarkerStart(
          "loadApplicationScript_StringConvert", "");
      break;
    case ReactMarker::JS_BUNDLE_STRING_CONVERT_STOP:
      HarmonyReactMarker::logMarkerFinish(
          "loadApplicationScript_StringConvert", "");
      break;
    case ReactMarker::NATIVE_MODULE_SETUP_START:
      HarmonyReactMarker::logMarkerStart("NATIVE_MODULE_SETUP", tag);
      break;
    case ReactMarker::NATIVE_MODULE_SETUP_STOP:
      HarmonyReactMarker::logMarkerFinish("NATIVE_MODULE_SETUP", tag);
      break;
    case ReactMarker::REGISTER_JS_SEGMENT_START:
      HarmonyReactMarker::logMarkerStart("REGISTER_JS_SEGMENT", tag);
      break;
    case ReactMarker::REGISTER_JS_SEGMENT_STOP:
      HarmonyReactMarker::logMarkerFinish("REGISTER_JS_SEGMENT", tag);
      break;
    case ReactMarker::NATIVE_REQUIRE_START:
    case ReactMarker::NATIVE_REQUIRE_STOP:
    case ReactMarker::REACT_INSTANCE_INIT_START:
    case ReactMarker::REACT_INSTANCE_INIT_STOP:
      break;
  }
}

void HarmonyReactMarker::setAppStartTime(double appStartTime) {
  sAppStartTime = appStartTime;
}

double HarmonyReactMarker::getAppStartTime() {
  return sAppStartTime;
}

} // namespace rnoh
