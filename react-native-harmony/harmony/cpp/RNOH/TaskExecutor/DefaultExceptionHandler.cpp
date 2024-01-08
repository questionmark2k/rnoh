#include "DefaultExceptionHandler.h"
#include <glog/logging.h>

void rnoh::defaultExceptionHandler(std::exception const &e) {
    LOG(ERROR) << "Exception thrown in task";
    LOG(ERROR) << e.what();
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception &nested) {
        LOG(ERROR) << nested.what();
    }
};