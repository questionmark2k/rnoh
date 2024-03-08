#include "TextComponentInstance.h"

#include <sstream>
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/text/ParagraphProps.h>
#include <react/renderer/components/text/ParagraphState.h>
#include <glog/logging.h>

using namespace rnoh;

TextComponentInstance::TextComponentInstance(Context context, facebook::react::Tag tag)
    : CppComponentInstance(std::move(context), tag) {}

void TextComponentInstance::setProps(facebook::react::Props::Shared props) {
    CppComponentInstance::setProps(props);
    auto textProps = std::dynamic_pointer_cast<const facebook::react::ParagraphProps>(props);
    if (textProps == nullptr) {
        return;
    }
    // TODO: text and paragraph attributes
}

void TextComponentInstance::setState(facebook::react::State::Shared state) {
    CppComponentInstance::setState(state);
    auto textState =
        std::dynamic_pointer_cast<const facebook::react::ConcreteState<facebook::react::ParagraphState>>(state);
    if (textState == nullptr) {
        return;
    }

    std::ostringstream contentStream;
    for (auto const &fragment : textState->getData().attributedString.getFragments()) {
        contentStream << fragment.string;
        m_textNode.setFontColor(fragment.textAttributes.foregroundColor).setFontSize(fragment.textAttributes.fontSize);
    }
    auto content = contentStream.str();
    m_textNode.setTextContent(content);
}

TextNode &TextComponentInstance::getLocalRootArkUINode() { return m_textNode; }
