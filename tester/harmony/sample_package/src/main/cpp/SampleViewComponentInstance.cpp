#include "SampleViewComponentInstance.h"

namespace rnoh {

SampleViewComponentInstance::SampleViewComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {
  m_stackNode.insertChild(m_textNode, 0);
  m_textNode.setTextContent(std::to_string(m_tag));
  m_textNode.setFontSize(m_fontSize);
  m_textNode.setFontColor(*facebook::react::whiteColor());
}

void SampleViewComponentInstance::onChildInserted(
    ComponentInstance::Shared const& childComponentInstance,
    std::size_t index) {
  CppComponentInstance::onChildInserted(childComponentInstance, index);
  m_stackNode.insertChild(
      childComponentInstance->getLocalRootArkUINode(), index + 1);
}

void SampleViewComponentInstance::onChildRemoved(
    ComponentInstance::Shared const& childComponentInstance) {
  CppComponentInstance::onChildRemoved(childComponentInstance);
  m_stackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
}

void SampleViewComponentInstance::onPropsChanged(
    SharedConcreteProps const& props) {
  CppComponentInstance::onPropsChanged(props);
  DLOG(INFO) << "SampleView props changed";
}

void SampleViewComponentInstance::handleCommand(
    std::string const& commandName,
    folly::dynamic const& args) {
  if (commandName == "toggleFontSize") {
    m_fontSize = m_fontSize == 12 ? 20 : 12;
    m_textNode.setFontSize(m_fontSize);
    return;
  }

  CppComponentInstance::handleCommand(commandName, args);
}

ArkUINode& SampleViewComponentInstance::getLocalRootArkUINode() {
  return m_stackNode;
}

} // namespace rnoh
