#include "TextComponentInstance.h"

#include <sstream>
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/text/ParagraphProps.h>
#include <react/renderer/components/text/ParagraphState.h>
#include <glog/logging.h>

namespace rnoh {

TextComponentInstance::TextComponentInstance(Context context, facebook::react::Tag tag)
    : ComponentInstance(std::move(context), tag) {}

void TextComponentInstance::setProps(facebook::react::Props::Shared props) {
    ComponentInstance::setProps(props);
    auto textProps = std::dynamic_pointer_cast<const facebook::react::ParagraphProps>(props);
    if (textProps == nullptr) {
        return;
    }
    // TODO: text and paragraph attributes
}

void TextComponentInstance::setState(facebook::react::State::Shared state) {
    ComponentInstance::setState(state);
    auto textState = std::dynamic_pointer_cast<const facebook::react::ConcreteState<facebook::react::ParagraphState>>(state);
    if (textState == nullptr) {
        return;
    }

    std::ostringstream contentStream;
    for (auto const &fragment : textState->getData().attributedString.getFragments()) {
        contentStream << fragment.string;
    }
    auto content = contentStream.str();
    m_textNode.setTextContent(content);
}

TextNode &TextComponentInstance::getLocalRootArkUINode() {
    return m_textNode;
}

} // namespace rnoh
