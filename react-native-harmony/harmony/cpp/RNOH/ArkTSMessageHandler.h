#pragma once
#include <string>
#include "RNInstance.h"

namespace rnoh {
/**
 * Provides a mechanism to respond to messages sent from the ArkTS side.
 * Messages are sent within the context of an RNInstance.
 */
class ArkTSMessageHandler {
 public:
  using Shared = std::shared_ptr<ArkTSMessageHandler>;

  struct Context {
    std::string messageName;
    folly::dynamic messagePayload;
    RNInstance::Weak rnInstance;
  };

  /**
   * This method is executed on MAIN thread.
   */
  virtual void handleArkTSMessage(const Context& ctx) = 0;
};
} // namespace rnoh