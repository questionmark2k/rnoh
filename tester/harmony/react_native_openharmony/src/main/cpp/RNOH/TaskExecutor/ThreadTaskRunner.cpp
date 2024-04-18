#include "ThreadTaskRunner.h"
#include <glog/logging.h>
#include <atomic>
#include <exception>

namespace rnoh {

ThreadTaskRunner::ThreadTaskRunner(
    std::string name,
    ExceptionHandler exceptionHandler)
    : name(name), exceptionHandler(std::move(exceptionHandler)) {
  thread = std::thread([this] { runLoop(); });
  auto handle = thread.native_handle();
  pthread_setname_np(handle, name.c_str());
}

ThreadTaskRunner::~ThreadTaskRunner() {
  LOG(INFO) << "Shutting down thread runner " << name;
  running = false;
  cv.notify_all();
  thread.join();
}

void ThreadTaskRunner::runAsyncTask(Task&& task) {
  {
    std::unique_lock<std::mutex> lock(mutex);
    asyncTaskQueue.emplace(std::move(task));
  }
  // NOTE: this should be fine:
  // the only threads waiting on the condition variable are the
  // runner thread and the thread that called runSyncTask,
  // and if some thread is waiting in runSyncTask,
  // the runner thread should be running and executing its task
  if (std::this_thread::get_id() != thread.get_id()) {
    cv.notify_one();
  }
}

void ThreadTaskRunner::runSyncTask(Task&& task) {
  if (isOnCurrentThread()) {
    task();
    return;
  }
  std::unique_lock<std::mutex> lock(mutex);
  std::atomic_bool done{false};
  syncTaskQueue.emplace([task = std::move(task), &done] {
    task();
    done = true;
  });
  cv.notify_all();
  cv.wait(lock, [this, &done] { return !running.load() || done.load(); });
}

bool ThreadTaskRunner::isOnCurrentThread() const {
  return std::this_thread::get_id() == thread.get_id();
}

void ThreadTaskRunner::setExceptionHandler(ExceptionHandler handler) {
  exceptionHandler = std::move(handler);
}

void ThreadTaskRunner::runLoop() {
  while (running) {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [this] { return hasPendingTasks() || !running; });
    if (!running) {
      return;
    }
    if (!syncTaskQueue.empty()) {
      auto task = std::move(syncTaskQueue.front());
      syncTaskQueue.pop();
      lock.unlock();
      try {
        task();
      } catch (std::exception const& e) {
        exceptionHandler(std::current_exception());
      }
      // notify the threads that called runSyncTask.
      // it's not enough to notify one thread,
      // because there could be multiple threads calling runSyncTask
      // at the same time
      cv.notify_all();
      continue;
    }
    if (!asyncTaskQueue.empty()) {
      auto task = std::move(asyncTaskQueue.front());
      asyncTaskQueue.pop();
      lock.unlock();
      try {
        task();
      } catch (std::exception const& e) {
        exceptionHandler(std::current_exception());
      }
    }
  }
}

} // namespace rnoh
