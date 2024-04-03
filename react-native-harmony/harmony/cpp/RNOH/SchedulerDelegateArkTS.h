#pragma once
#include <folly/dynamic.h>
#include <react/renderer/scheduler/SchedulerDelegate.h>

#include "RNOH/ArkTSChannel.h"
#include "RNOH/MountingManager.h"

namespace rnoh {

class SchedulerDelegateArkTS : public facebook::react::SchedulerDelegate {
 public:
  using Unique = std::unique_ptr<SchedulerDelegateArkTS>;

  SchedulerDelegateArkTS(
      MountingManager::Shared mountingManager,
      ArkTSChannel::Shared arkTsChannel)
      : mountingManager(std::move(mountingManager)),
        m_arkTsChannel(arkTsChannel){};

  ~SchedulerDelegateArkTS() = default;

  void schedulerDidFinishTransaction(
      facebook::react::MountingCoordinator::Shared mountingCoordinator)
      override {
    mountingManager->scheduleTransaction(mountingCoordinator);
  }

  void schedulerDidRequestPreliminaryViewAllocation(
      facebook::react::SurfaceId surfaceId,
      const facebook::react::ShadowNode& shadowView) override {}

  void schedulerDidDispatchCommand(
      const facebook::react::ShadowView& shadowView,
      std::string const& commandName,
      folly::dynamic const& args) override {
    mountingManager->dispatchCommand(shadowView.tag, commandName, args);
  }

  void schedulerDidSendAccessibilityEvent(
      const facebook::react::ShadowView& shadowView,
      std::string const& eventType) override {}

  void schedulerDidSetIsJSResponder(
      facebook::react::ShadowView const& shadowView,
      bool isJSResponder,
      bool blockNativeResponder) override {
    folly::dynamic payload = folly::dynamic::object;
    payload["tag"] = shadowView.tag;
    payload["isJSResponder"] = isJSResponder;
    payload["blockNativeResponder"] = blockNativeResponder;
    m_arkTsChannel->postMessage("SCHEDULER_DID_SET_IS_JS_RESPONDER", payload);
  }

 private:
  MountingManager::Shared mountingManager;
  ArkTSChannel::Shared m_arkTsChannel;
};

} // namespace rnoh
