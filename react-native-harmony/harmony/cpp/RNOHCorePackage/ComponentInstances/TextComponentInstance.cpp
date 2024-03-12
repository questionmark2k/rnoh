#include "TextComponentInstance.h"

#include <sstream>
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/text/ParagraphProps.h>
#include <react/renderer/components/text/ParagraphState.h>
#include <glog/logging.h>

using namespace rnoh;

TextComponentInstance::TextComponentInstance(Context context, facebook::react::Tag tag)
    : CppComponentInstance(std::move(context), tag) {}

void TextComponentInstance::onPropsChanged(std::shared_ptr<const facebook::react::ParagraphProps> const &props) {
    CppComponentInstance::onPropsChanged(props);
}

void TextComponentInstance::onStateChanged(std::shared_ptr<const facebook::react::ConcreteState<facebook::react::ParagraphState>> const &textState) {
    std::ostringstream contentStream;
    for (auto const &fragment : textState->getData().attributedString.getFragments()) {
        contentStream << fragment.string;
        m_textNode.setFontColor(fragment.textAttributes.foregroundColor).setFontSize(fragment.textAttributes.fontSize);
    }
    auto content = contentStream.str();
    m_textNode.setTextContent(content);
}

TextNode &TextComponentInstance::getLocalRootArkUINode() { return m_textNode; }
