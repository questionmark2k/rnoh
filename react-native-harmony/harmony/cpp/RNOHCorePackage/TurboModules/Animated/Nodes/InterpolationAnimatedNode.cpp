#include "InterpolationAnimatedNode.h"
#include "RNOH/Color.h"
#include "glog/logging.h"

using namespace facebook;

namespace rnoh {

size_t findRangeIndex(double value, folly::dynamic const& inputRange) {
  size_t index = 1;
  while (index < inputRange.size() - 1 &&
         inputRange[index].asDouble() < value) {
    index++;
  }
  return index - 1;
}

InterpolationAnimatedNode::InterpolationAnimatedNode(
    folly::dynamic const& config,
    AnimatedNodesManager& nodesManager)
    : m_nodesManager(nodesManager) {
  m_inputRange = config["inputRange"];
  m_outputRange = config["outputRange"];
  m_extrapolateLeft =
      extrapolateTypeFromString(config["extrapolateLeft"].asString());
  m_extrapolateRight =
      extrapolateTypeFromString(config["extrapolateRight"].asString());

  m_outputType = OutputType::Number;
  if (!config["outputType"].empty()) {
    auto outputType = config["outputType"].asString();
    if (outputType == "number") {
      m_outputType = OutputType::Number;
    } else if (outputType == "color") {
      m_outputType = OutputType::Color;
    } else if (outputType == "string") {
      m_outputType = OutputType::String;
    }
  }
}

void InterpolationAnimatedNode::update() {
  if (m_parent == std::nullopt) {
    // this can occur if the graph is still being constructed
    // when an update fires - just ignore it
    return;
  }

  auto& parentNode = getParentNode();
  double value = parentNode.getValue();

  auto rangeIndex = findRangeIndex(value, m_inputRange);

  m_value = interpolate(
      value,
      m_inputRange[rangeIndex].asDouble(),
      m_inputRange[rangeIndex + 1].asDouble(),
      m_outputRange[rangeIndex].asDouble(),
      m_outputRange[rangeIndex + 1].asDouble(),
      m_extrapolateLeft,
      m_extrapolateRight);

  switch (m_outputType) {
    case OutputType::Number:
      m_value = interpolate(
          value,
          m_inputRange[rangeIndex].asDouble(),
          m_inputRange[rangeIndex + 1].asDouble(),
          m_outputRange[rangeIndex].asDouble(),
          m_outputRange[rangeIndex + 1].asDouble(),
          m_extrapolateLeft,
          m_extrapolateRight);
      break;
    case OutputType::String:
      // TODO: implement
      m_value = value;
      break;
    case OutputType::Color:
      auto colorA = Color::from(m_outputRange[rangeIndex].asInt());
      auto colorB = Color::from(m_outputRange[rangeIndex + 1].asInt());
      auto mixValue = (value - m_inputRange[rangeIndex].asDouble()) /
          (m_inputRange[rangeIndex + 1].asDouble() -
           m_inputRange[rangeIndex].asDouble());
      auto clampedMixValue = std::max(std::min(mixValue, 1.0), 0.0);
      auto newColor = colorA * (1 - clampedMixValue) + colorB * clampedMixValue;
      m_value = newColor.asColorValue();
      break;
  }
}

void InterpolationAnimatedNode::onAttachedToNode(facebook::react::Tag tag) {
  m_nodesManager.getValueNodeByTag(tag);
  m_parent = tag;
}

void InterpolationAnimatedNode::onDetachedFromNode(facebook::react::Tag tag) {
  if (tag != m_parent) {
    throw std::runtime_error(
        "Trying to detach animated node from node " + std::to_string(tag) +
        " which is not its parent");
  }
  m_parent = std::nullopt;
}

double InterpolationAnimatedNode::interpolate(
    double value,
    double inputMin,
    double inputMax,
    double outputMin,
    double outputMax,
    ExtrapolateType extrapolateLeft,
    ExtrapolateType extrapolateRight) const {
  double result = value;

  if (result < inputMin) {
    if (extrapolateLeft == ExtrapolateType::EXTRAPOLATE_TYPE_IDENTITY) {
      return result;
    } else if (extrapolateLeft == ExtrapolateType::EXTRAPOLATE_TYPE_CLAMP) {
      result = inputMin;
    } else if (extrapolateLeft == ExtrapolateType::EXTRAPOLATE_TYPE_EXTEND) {
      // noop
    } else {
      // this should never happen, since we've already checked the extrapolation
      // type
      throw std::runtime_error(
          "Invalid extrapolation type for left extrapolation");
    }
  }

  if (result > inputMax) {
    if (extrapolateRight == ExtrapolateType::EXTRAPOLATE_TYPE_IDENTITY) {
      return result;
    } else if (extrapolateRight == ExtrapolateType::EXTRAPOLATE_TYPE_CLAMP) {
      result = inputMax;
    } else if (extrapolateRight == ExtrapolateType::EXTRAPOLATE_TYPE_EXTEND) {
      // noop
    } else {
      // this should never happen
      throw std::runtime_error(
          "Invalid extrapolation type for right extrapolation");
    }
  }

  if (outputMin == outputMax) {
    return outputMin;
  }

  if (inputMin == inputMax) {
    if (value >= inputMax) {
      return outputMax;
    } else {
      return outputMin;
    }
  }

  double inputRange = inputMax - inputMin;
  double outputRange = outputMax - outputMin;

  return outputMin + outputRange * (result - inputMin) / inputRange;
}

ValueAnimatedNode& InterpolationAnimatedNode::getParentNode() const {
  if (m_parent == std::nullopt) {
    throw std::runtime_error("Parent animated node has not been set");
  }
  return m_nodesManager.getValueNodeByTag(m_parent.value());
}

InterpolationAnimatedNode::ExtrapolateType
InterpolationAnimatedNode::extrapolateTypeFromString(
    std::string const& extrapolateType) {
  if (extrapolateType == "identity") {
    return ExtrapolateType::EXTRAPOLATE_TYPE_IDENTITY;
  } else if (extrapolateType == "clamp") {
    return ExtrapolateType::EXTRAPOLATE_TYPE_CLAMP;
  } else if (extrapolateType == "extend") {
    return ExtrapolateType::EXTRAPOLATE_TYPE_EXTEND;
  } else {
    throw std::runtime_error(
        "Invalid extrapolation type " + extrapolateType + " provided.");
  }
}

} // namespace rnoh
