#include <uv.h>
#include <glog/logging.h>

#include "TaskExecutor.h"
#include "ThreadTaskRunner.h"
#include "NapiTaskRunner.h"

namespace rnoh {

TaskExecutor::TaskExecutor(napi_env mainEnv, bool shouldEnableBackground) {
    auto mainTaskRunner = std::make_shared<NapiTaskRunner>(mainEnv);
    auto jsTaskRunner = std::make_shared<ThreadTaskRunner>("RNOH_JS");
    auto backgroundExecutor = shouldEnableBackground ? std::make_shared<ThreadTaskRunner>("RNOH_BACKGROUND") : nullptr;
    m_taskRunners = {
        mainTaskRunner,
        jsTaskRunner,
        backgroundExecutor};
}

void TaskExecutor::runTask(TaskThread thread, Task &&task) {
    m_taskRunners[thread]->runAsyncTask(std::move(task));
}

void TaskExecutor::runSyncTask(TaskThread thread, Task &&task) {
    auto waitsOnThread = m_waitsOnThread[thread];
    if (waitsOnThread.has_value() && isOnTaskThread(waitsOnThread.value())) {
        throw std::runtime_error("Deadlock detected");
    }
    auto currentThread = getCurrentTaskThread();
    if (currentThread.has_value()) {
        m_waitsOnThread[currentThread.value()] = thread;
    }
    std::exception_ptr thrownError;
    m_taskRunners[thread]->runSyncTask([task = std::move(task), &thrownError]() {
        try {
            task();
        } catch (const std::exception &e) {
            thrownError = std::current_exception();
        }
    });
    if (thrownError) {
        std::rethrow_exception(thrownError);
    }
    if (currentThread.has_value()) {
        m_waitsOnThread[currentThread.value()] = std::nullopt;
    }
}

bool TaskExecutor::isOnTaskThread(TaskThread thread) const {
    auto runner = m_taskRunners[thread];
    return runner && runner->isOnCurrentThread();
}

std::optional<TaskThread> TaskExecutor::getCurrentTaskThread() const {
    if (isOnTaskThread(TaskThread::MAIN)) {
        return TaskThread::MAIN;
    } else if (isOnTaskThread(TaskThread::JS)) {
        return TaskThread::JS;
    } else if (isOnTaskThread(TaskThread::BACKGROUND)) {
        return TaskThread::BACKGROUND;
    } else {
        return std::nullopt;
    }
}

void TaskExecutor::setExceptionHandler(ExceptionHandler handler) {
    for (auto &taskRunner : m_taskRunners) {
        if (taskRunner) {
            taskRunner->setExceptionHandler(handler);
        }
    }
}

} // namespace rnoh
