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
            if (m_props != nullptr) {
                auto props = m_props;
                return point.x >= -props->hitSlop.left && point.y >= -props->hitSlop.top &&
                    point.x < m_layoutMetrics.frame.size.width + props->hitSlop.right &&
                    point.y < m_layoutMetrics.frame.size.height + props->hitSlop.bottom;
            }
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
            auto old = m_props;
            if (props != nullptr) {
                if (!old || *(props->backgroundColor) != *(old->backgroundColor)) {
                    this->getLocalRootArkUINode().setBackgroundColor(props->backgroundColor);
                }

                facebook::react::BorderMetrics borderMetrics = props->resolveBorderMetrics(this->m_layoutMetrics);
                facebook::react::BorderMetrics oldBorderMetrics;
                if (!old || borderMetrics.borderWidths != m_oldBorderMetrics.borderWidths) {
                    this->getLocalRootArkUINode().setBorderWidth(borderMetrics.borderWidths);
                }
                if (!old || borderMetrics.borderColors != m_oldBorderMetrics.borderColors) {
                    this->getLocalRootArkUINode().setBorderColor(borderMetrics.borderColors);
                }
                if (!old || borderMetrics.borderRadii != m_oldBorderMetrics.borderRadii || !m_isRadiusSetValid) {
                    if (this->m_layoutMetrics.frame.size != facebook::react::Size{0,0}) {
                        m_isRadiusSetValid = true;
                    }
                    this->getLocalRootArkUINode().setBorderRadius(borderMetrics.borderRadii);
                }
                if (!old || borderMetrics.borderStyles != m_oldBorderMetrics.borderStyles) {
                    this->getLocalRootArkUINode().setBorderStyle(borderMetrics.borderStyles);
                }

                if (!old || props->shadowColor != old->shadowColor 
                    && props->shadowOffset != old->shadowOffset 
                    && props->shadowOpacity != old->shadowOpacity
                    && props->shadowRadius != old->shadowRadius) {
                    this->getLocalRootArkUINode().setShadow(props->shadowColor, props->shadowOffset,
                        props->shadowOpacity, props->shadowRadius);
                }

                if (!old || props->transform != old->transform ||
                    abs(m_oldPointScaleFactor - m_layoutMetrics.pointScaleFactor) > 0.001f) {
                    m_oldPointScaleFactor = m_layoutMetrics.pointScaleFactor;
                    this->getLocalRootArkUINode().setTransform(props->transform, m_layoutMetrics.pointScaleFactor);
                }

                if (!old || props->pointerEvents != old->pointerEvents) {
                    this->getLocalRootArkUINode().setHitTestMode(props->pointerEvents);
                }

                if (!old || props->accessibilityHint != old->accessibilityHint) {
                    this->getLocalRootArkUINode().setAccessibilityDescription(props->accessibilityHint);
                }

                if (!old || props->importantForAccessibility != old->importantForAccessibility) {
                    this->getLocalRootArkUINode().setAccessibilityLevel(props->importantForAccessibility);
                }

                if (!old || props->accessibilityLabel != old->accessibilityLabel) {
                    this->getLocalRootArkUINode().setAccessibilityText(props->accessibilityLabel);
                }
            
                if (!old || props->accessible != old->accessible) {
                    this->getLocalRootArkUINode().setAccessibilityGroup(props->accessible);
                }

                if (!old) {
                    this->getLocalRootArkUINode().setId(getTag());
                }

                if (!old || props->opacity != old->opacity || props->transform != old->transform || props->backfaceVisibility != old->backfaceVisibility) {
                    this->setOpacity(props);
                }

                auto newOverflow = props->getClipsContentToBounds();
                if (!old || (old->getClipsContentToBounds() != newOverflow)) {
                    this->getLocalRootArkUINode().setClip(newOverflow);
                }
                m_oldBorderMetrics = props->resolveBorderMetrics(this->m_layoutMetrics);
            }
        };

        virtual void onStateChanged(SharedConcreteState const &state){};

        virtual void onEventEmitterChanged(SharedConcreteEventEmitter const &eventEmitter){};
    private:
        void setOpacity(SharedConcreteProps const &props) {
            auto opacity = props->opacity;
            float validOpacity = std::max(0.0f, std::min((float)opacity, 1.0f));
            auto matrix = props->transform.matrix;
            if (props->backfaceVisibility == facebook::react::BackfaceVisibility::Hidden) {
                std::array<int32_t, 4> vec{{0, 0, 1, 0}};
                std::array<int32_t, 4> resVec{{0, 0, 0, 0}};
                for (int32_t i = 0; i < 4; ++i) {
                    for (int32_t j = 0; j < 4; ++j) {
                    resVec[i] += matrix[i * 4 + j] * vec[j];
                    }
                }
                if (resVec[2] < 0.0) {
                    validOpacity = 0.0;
                }
            }
            this->getLocalRootArkUINode().setOpacity(validOpacity);
        }

    protected:
        SharedConcreteProps m_props;
        SharedConcreteState m_state;
        SharedConcreteEventEmitter m_eventEmitter;
    };
} // namespace rnoh
