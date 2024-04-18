#pragma once
#include <functional>
#include <mutex>
#include <string>

#include <cxxreact/ReactMarker.h>
#include "hitrace/trace.h"

using namespace facebook::react;

namespace rnoh {
class HarmonyReactMarker {
 public:
  static void setLogPerfMarkerIfNeeded();
  static void logMarker(const std::string& marker);
  static void setAppStartTime(double startTime);

 private:
  static inline double sAppStartTime = 0.0;
  static void logMarkerStart(const std::string& marker, const std::string& tag);
  static void logMarkerFinish(
      const std::string& marker,
      const std::string& tag);

  static void logPerfMarker(
      const ReactMarker::ReactMarkerId markerId,
      const char* tag);
  static double getAppStartTime();
};

} // namespace rnoh
