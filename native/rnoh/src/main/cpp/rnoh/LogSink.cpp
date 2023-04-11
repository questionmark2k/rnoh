#include <hilog/log.h>
#include "RNOH/LogSink.h"


LogSink *LogSink::instance = nullptr;

void LogSink::initializeLogging() {
    if (!instance) {
        instance = new LogSink();
        google::AddLogSink(instance);
        FLAGS_logtostderr = true;
        google::InitGoogleLogging("[RNOH]");
    }
}

void LogSink::send(
    google::LogSeverity severity,
    const char *full_filename,
    const char *base_filename,
    int line,
    const ::tm *tm_time,
    const char *message,
    size_t message_len) {
    std::ostringstream stream;
    stream  << "[RNOH] "
            << "<" << base_filename << ':' << line << "> "
            << std::string(message, message_len);
    auto messageString = stream.str();
    auto c_str = messageString.c_str();

    switch (severity) {
        case google::GLOG_INFO:
            OH_LOG_INFO(LOG_APP, "%{public}s", c_str);
            break;
        case google::GLOG_WARNING:
            OH_LOG_WARN(LOG_APP, "%{public}s", c_str);
            break;
        case google::GLOG_ERROR:
            OH_LOG_ERROR(LOG_APP, "%{public}s", c_str);
            break;
        case google::GLOG_FATAL:
            OH_LOG_FATAL(LOG_APP, "%{public}s", c_str);
            break;
        default:
            OH_LOG_WARN(LOG_APP, "%{public}s", c_str);
            break;
    }
}