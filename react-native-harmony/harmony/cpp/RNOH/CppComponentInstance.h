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
#include "RNOH/ComponentInstance.h"


namespace rnoh {

    class CppComponentInstance : public ComponentInstance {
    public:
        using Shared = std::shared_ptr<CppComponentInstance>;

        CppComponentInstance(Context context, facebook::react::Tag tag);

        facebook::react::Tag getTag() const { return m_tag; }

        void setProps(facebook::react::Props::Shared props) override;

        void setState(facebook::react::State::Shared state) override {};

        void setLayout(facebook::react::LayoutMetrics layoutMetrics) override;

        void setEventEmitter(facebook::react::SharedEventEmitter eventEmitter) override {
            m_cppComponentEventEmitter = eventEmitter;
        };

        // TouchTarget implementation
        facebook::react::LayoutMetrics getLayoutMetrics() const override;

        facebook::react::Point computeChildPoint(facebook::react::Point const &point,
                                                 TouchTarget::Shared const &child) const override;

        bool containsPoint(facebook::react::Point const &point) const override;

        bool containsPointInBoundingBox(facebook::react::Point const &point) const override;

        facebook::react::Tag getTouchTargetTag() const override { return getTag(); }

        facebook::react::SharedTouchEventEmitter getTouchEventEmitter() const override { 
            auto ret = std::dynamic_pointer_cast<const facebook::react::TouchEventEmitter>(m_cppComponentEventEmitter);
            return ret; 
        }

        std::vector<TouchTarget::Shared> getTouchTargetChildren() const override {
            auto children = getChildren();
            return std::vector<TouchTarget::Shared>(children.begin(), children.end());
        }

    private:
        facebook::react::SharedEventEmitter m_cppComponentEventEmitter;

    };

} // namespace rnoh