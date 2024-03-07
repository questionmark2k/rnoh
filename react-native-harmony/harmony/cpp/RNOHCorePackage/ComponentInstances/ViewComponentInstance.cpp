#include "ViewComponentInstance.h"

namespace rnoh {

    ViewComponentInstance::ViewComponentInstance(Context context, facebook::react::Tag tag)
        : ComponentInstance(std::move(context), tag) {}

    void ViewComponentInstance::setEventEmitter(facebook::react::SharedEventEmitter eventEmitter) {
        m_eventEmitter = std::dynamic_pointer_cast<facebook::react::ViewEventEmitter const>(eventEmitter);
    }

    facebook::react::SharedViewEventEmitter const &ViewComponentInstance::getEventEmitter() const {
        return m_eventEmitter;
    }

    void ViewComponentInstance::insertChild(ComponentInstance::Shared childComponentInstance, std::size_t index) {
        ComponentInstance::insertChild(childComponentInstance, index);
        m_stackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
    }

    void ViewComponentInstance::removeChild(ComponentInstance::Shared childComponentInstance) {
        ComponentInstance::removeChild(childComponentInstance);
        m_stackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
    };

    StackNode &ViewComponentInstance::getLocalRootArkUINode() { return m_stackNode; }

    facebook::react::SharedTouchEventEmitter ViewComponentInstance::getTouchEventEmitter() const
    {
        return getEventEmitter();
    }
} // namespace rnoh
