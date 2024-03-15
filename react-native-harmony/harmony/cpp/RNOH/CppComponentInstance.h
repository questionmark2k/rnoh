/**
 * Used only in C-API based Architecture.
 */
#pragma once
#include <vector>
#include <react/renderer/core/LayoutMetrics.h>
#include <react/renderer/core/Props.h>
#include <react/renderer/core/State.h>
#include <react/renderer/core/EventEmitter.h>
#include <react/renderer/core/ReactPrimitives.h>
#include <react/renderer/components/view/ViewProps.h>
#include <react/renderer/components/view/TouchEventEmitter.h>
#include "RNOH/ComponentInstance.h"


namespace rnoh {
    template <typename ShadowNodeT> class CppComponentInstance : public ComponentInstance {
        static_assert(std::is_base_of_v<facebook::react::ShadowNode, ShadowNodeT>,
                      "ShadowNodeT must be a subclass of facebook::react::ShadowNode");

    protected:
        using ConcreteProps = typename ShadowNodeT::ConcreteProps;
        using ConcreteState = typename ShadowNodeT::ConcreteState;
        using ConcreteEventEmitter = typename ShadowNodeT::ConcreteEventEmitter;
        using SharedConcreteProps = std::shared_ptr<const ConcreteProps>;
        using SharedConcreteState = std::shared_ptr<const ConcreteState>;
        using SharedConcreteEventEmitter = std::shared_ptr<const ConcreteEventEmitter>;

    public:
        using Shared = std::shared_ptr<CppComponentInstance>;

        CppComponentInstance(Context context) : ComponentInstance(std::move(context)) {}

        facebook::react::Tag getTag() const { return m_tag; }

        facebook::react::Props::Shared getProps() const override { return m_props; }

        void setProps(facebook::react::Props::Shared props) final {
            auto newProps = std::dynamic_pointer_cast<const ConcreteProps>(props);
            if (!newProps) {
                return;
            }

            this->onPropsChanged(newProps);
            m_props = newProps;
        }

        SharedConcreteState const &getState() const { return m_state; }

        void setState(facebook::react::State::Shared state) final {
            auto newState = std::dynamic_pointer_cast<const ConcreteState>(state);
            if (!newState) {
                return;
            }

            this->onStateChanged(newState);
            m_state = newState;
        }

        SharedConcreteEventEmitter const &getEventEmitter() const { return m_eventEmitter; }

        void setEventEmitter(facebook::react::SharedEventEmitter eventEmitter) final {
            auto newEventEmitter = std::dynamic_pointer_cast<const ConcreteEventEmitter>(eventEmitter);
            if (!newEventEmitter) {
                return;
            }
            this->onEventEmitterChanged(newEventEmitter);
            m_eventEmitter = newEventEmitter;
        }

        void setLayout(facebook::react::LayoutMetrics layoutMetrics) override {
            this->getLocalRootArkUINode().setPosition(layoutMetrics.frame.origin);
            this->getLocalRootArkUINode().setSize(layoutMetrics.frame.size);
            m_layoutMetrics = layoutMetrics;
        }

        // TouchTarget implementation
        facebook::react::LayoutMetrics getLayoutMetrics() const override { return m_layoutMetrics; }

        facebook::react::Point computeChildPoint(facebook::react::Point const &point,
                                                 TouchTarget::Shared const &child) const override {
            auto childLayout = child->getLayoutMetrics();

            // TODO: apply inverse transform

            return point - childLayout.frame.origin;
        }

        bool containsPoint(facebook::react::Point const &point) const override {
            // TODO: hitslops
            return point.x >= 0 && point.y >= 0 && point.x < m_layoutMetrics.frame.size.width &&
                   point.y < m_layoutMetrics.frame.size.height;
        }

        bool containsPointInBoundingBox(facebook::react::Point const &point) const override {
            return containsPoint(point);
        }

        facebook::react::Tag getTouchTargetTag() const override { return getTag(); }

        facebook::react::SharedTouchEventEmitter getTouchEventEmitter() const override { return m_eventEmitter; }

        std::vector<TouchTarget::Shared> getTouchTargetChildren() const override {
            auto children = getChildren();
            return std::vector<TouchTarget::Shared>(children.begin(), children.end());
        }

    protected:
        virtual void onPropsChanged(SharedConcreteProps const &props) {
            this->getLocalRootArkUINode().setBackgroundColor(props->backgroundColor);

            facebook::react::BorderMetrics borderMetrics = props->resolveBorderMetrics(this->m_layoutMetrics);
            this->getLocalRootArkUINode().setBorderWidth(borderMetrics.borderWidths);
            this->getLocalRootArkUINode().setBorderColor(borderMetrics.borderColors);
            this->getLocalRootArkUINode().setBorderRadius(borderMetrics.borderRadii);
            this->getLocalRootArkUINode().setBorderStyle(borderMetrics.borderStyles);

            this->getLocalRootArkUINode().setOpacity(props->opacity);
            this->getLocalRootArkUINode().setShadow(props->shadowColor, props->shadowOffset, props->shadowOpacity, props->shadowRadius);
            this->getLocalRootArkUINode().setTransform(props->transform, m_layoutMetrics.pointScaleFactor);
            this->getLocalRootArkUINode().setHitTestMode(props->pointerEvents);
            this->getLocalRootArkUINode().setAccessibilityDescription(props->accessibilityHint);
            this->getLocalRootArkUINode().setAccessibilityLevel(props->importantForAccessibility);
            this->getLocalRootArkUINode().setAccessibilityText(props->accessibilityLabel);
            this->getLocalRootArkUINode().setAccessibilityGroup(props->accessible);
            this->getLocalRootArkUINode().setId(m_tag);
            this->getLocalRootArkUINode().setClip(props->getClipsContentToBounds());
        };

        virtual void onStateChanged(SharedConcreteState const &state){};

        virtual void onEventEmitterChanged(SharedConcreteEventEmitter const &eventEmitter){};

        SharedConcreteProps m_props;
        SharedConcreteState m_state;
        SharedConcreteEventEmitter m_eventEmitter;
    };
} // namespace rnoh
