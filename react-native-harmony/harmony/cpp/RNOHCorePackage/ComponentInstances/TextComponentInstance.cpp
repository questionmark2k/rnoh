#include "TextComponentInstance.h"
#include "react/renderer/components/view/conversions.h"
#include "react/renderer/attributedstring/primitives.h"
#include <sstream>
#include <react/renderer/core/ConcreteState.h>
#include <react/renderer/components/text/ParagraphProps.h>
#include <react/renderer/components/text/ParagraphState.h>
#include <glog/logging.h>

using namespace rnoh;

const static std::string IMAGE_SPAN_FLAG = "￼";
const static float DEFAULT_LINE_SPACING = 0.15f;
float DEFAULT_FONT_SIZE = 16.0f;

TextComponentInstance::TextComponentInstance(Context context) : CppComponentInstance(std::move(context)) {
    m_stackNode.insertChild(m_textNode, 0);
    m_children.clear();
}

TextComponentInstance::~TextComponentInstance() {
    for (auto item : m_children) {
        m_textNode.removeChild(*item);
    }
    m_children.clear();
}

void TextComponentInstance::onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) {
    m_stackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
}

void TextComponentInstance::onChildRemoved(ComponentInstance::Shared const &childComponentInstance) {
    m_stackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
}

void TextComponentInstance::onPropsChanged(std::shared_ptr<const facebook::react::ParagraphProps> const &textProps) {
    CppComponentInstance::onPropsChanged(textProps);
    if (textProps == nullptr) {
        return;
    }
    
    // padding
    TextPaddingInfo info = TextConversions::getArkUIPadding(textProps);
    VLOG(3) << "[text-debug] textProps->setPadding top=" << info.top << ", right=" << info.right
              << ", bottom=" << info.bottom << ", left=" << info.left;
    m_textNode.setPadding(info.top, info.right, info.bottom, info.left);
    
    // Copy Option
    VLOG(3) << "[text-debug] textProps->isSelectable=" << textProps->isSelectable;
    int32_t testCopyOption = ArkUI_CopyOptions::ARKUI_COPY_OPTIONS_NONE;
    if (textProps->isSelectable) {
        testCopyOption = ArkUI_CopyOptions::ARKUI_COPY_OPTIONS_LOCAL_DEVICE;
    }
    m_textNode.setTextCopyOption(testCopyOption);
    
    // MaxFontSize
    std::optional<float> maxFontSize = TextConversions::getMaxFontSize(textProps);
    if (maxFontSize.has_value()) {
        m_textNode.setMaxFontSize(maxFontSize.value());
        VLOG(3) << "[text-debug] maxFontSize=" << maxFontSize.value();
    }
    
    if (textProps->rawProps != nullptr) {
        // stack align
        facebook::react::TextAlignment alignHorizon = facebook::react::TextAlignment::Left;
        std::string alignVertical = "top";
        if (textProps->rawProps.count("textAlignVertical") != 0) {
            alignVertical = textProps->rawProps["textAlignVertical"].asString();
        }
        if (textProps->textAttributes.alignment.has_value()) {
            alignHorizon = textProps->textAttributes.alignment.value();
        }
        int32_t stackAlign = TextConversions::getStackAlign(alignHorizon, alignVertical);

        facebook::react::TextAlignment alignHorizonOld = facebook::react::TextAlignment::Left;
        std::string alignVerticalOld = "top";
        if (m_props && m_props->rawProps.count("textAlignVertical") != 0) {
            alignVerticalOld = m_props->rawProps["textAlignVertical"].asString();
        }
        if (m_props && m_props->textAttributes.alignment.has_value()) {
            alignHorizonOld = m_props->textAttributes.alignment.value();
        }
        int32_t stackAlignOld = TextConversions::getStackAlign(alignHorizonOld, alignVerticalOld);

        if (!m_props || stackAlign != stackAlignOld) {
            VLOG(3) << "[text-debug] stackAlign=" << stackAlign << ", stackAlignOld=" << stackAlignOld;
            m_stackNode.setAlign(stackAlign);
        }

        // enable
        if (textProps->rawProps.count("disabled") != 0) {
            bool enableFlag = textProps->rawProps["disabled"].asBool();
            VLOG(3) << "[text-debug] rawProps->disabled=" << enableFlag;
            m_textNode.setTextEnable(!enableFlag);
        }

        // MinFontSize
        std::optional<float> minFontSize = TextConversions::getMinFontSize(textProps);
        if (minFontSize.has_value()) {
            m_textNode.setMinFontSize(minFontSize.value());
            VLOG(3) << "[text-debug] minFontSize=" << minFontSize.value();
        }
    }
    VLOG(3) << "[text-debug] setProps end";
}

void TextComponentInstance::onStateChanged(
    std::shared_ptr<const facebook::react::ConcreteState<facebook::react::ParagraphState>> const &textState) {
    CppComponentInstance::onStateChanged(textState);
    if (textState == nullptr) {
        return;
    }
    if (textState->getData().attributedString.getFragments().size() <= 0) {
        return;
    }
    for (auto item : m_children) {
        m_textNode.removeChild(*item);
    }
    m_children.clear();
    m_childIndex = 0;
    VLOG(3) << "[text-debug] getFragments size:" << textState->getData().attributedString.getFragments().size();
    for (auto fragment : textState->getData().attributedString.getFragments()) {
        if (fragment.string != IMAGE_SPAN_FLAG) {
            std::shared_ptr<SpanNode> spanNode = std::make_shared<SpanNode>();
            VLOG(3) << "[text-debug] create span text=" << fragment.string.c_str() << ", index=" << m_childIndex;
            m_textNode.insertChild(*spanNode, m_childIndex);
            m_children.push_back(spanNode);
            setFragment(fragment, spanNode, m_childIndex);
            m_childIndex++;
        } else {
            VLOG(3) << "[text-debug] create image span, index=" << m_childIndex;
            std::shared_ptr<ImageSpanNode> imageSpanNode = std::make_shared<ImageSpanNode>();
            m_textNode.insertChild(*imageSpanNode, m_childIndex);
            m_children.push_back(imageSpanNode);
            setImageSpanSize(fragment.parentShadowView.layoutMetrics.frame.size, imageSpanNode);
            m_childIndex++;
        }
    }
    this->setTextAttributes(textState->getData().attributedString.getFragments()[0].textAttributes);
    this->setParagraphAttributes(textState->getData().paragraphAttributes);
}

void TextComponentInstance::setTextAttributes(const facebook::react::TextAttributes &textAttributes) {
    // TextAlign
    if (textAttributes.alignment.has_value()) {
        int32_t align = TextConversions::getArkUITextAlignment(textAttributes.alignment.value());
        VLOG(3) << "[text-debug] textAttributes.alignment=" << align;
        m_textNode.setTextAlign(align);
    }
}

void TextComponentInstance::setFragment(const facebook::react::AttributedString::Fragment &fragment,
                                        std::shared_ptr<SpanNode> spanNode, uint32_t index) {
    if (spanNode == nullptr) {
        return;
    }

    facebook::react::TextAttributes textAttributes = fragment.textAttributes;
    spanNode->setSpanContent(fragment.string);
    VLOG(3) << "[text-debug] fontColor=" << (uint32_t)(*textAttributes.foregroundColor);
    if (textAttributes.foregroundColor) {
        spanNode->setFontColor((uint32_t)(*textAttributes.foregroundColor));
    }
    VLOG(3) << "[text-debug] fontFamily=" << textAttributes.fontFamily;
    spanNode->setFontFamily(textAttributes.fontFamily);

    if (textAttributes.fontWeight.has_value()) {
        int32_t realFontWeight = TextConversions::getArkUIFontWeight((int32_t)textAttributes.fontWeight.value());
        VLOG(3) << "[text-debug] realFontWeight=" << realFontWeight << ", index=" << index;
        spanNode->setFontWeight(realFontWeight);
    }

    // FontSize
    float fontSize = isnan(textAttributes.fontSize) ? DEFAULT_FONT_SIZE : (float)textAttributes.fontSize;
    VLOG(3) << "[text-debug] textAttributes.fontSize=" << textAttributes.fontSize << ", fontSize=" << fontSize
            << ", index=" << index;
    spanNode->setFontSize(fontSize);

    // FontStyle
    if (textAttributes.fontStyle.has_value()) {
        VLOG(3) << "[text-debug] textAttributes.fontStyle=" << (int32_t)textAttributes.fontStyle.value()
                  << ", index=" << index;
        spanNode->setFontStyle((int32_t)textAttributes.fontStyle.value());
    }

    // TextDecoration
    if (textAttributes.textDecorationLineType.has_value()) {
        int32_t type = (int32_t)textAttributes.textDecorationLineType.value(); 
        VLOG(3) << "[text-debug] textAttributes.textDecorationLineType="
                          << type
                          << ", textAttributes.textDecorationColor=" << (uint32_t)(*textAttributes.textDecorationColor)
                          << ", index=" << index;
        uint32_t color = 0xFF000000;
        if (textAttributes.textDecorationColor) {
            color = (uint32_t)(*textAttributes.textDecorationColor);
        } else if (textAttributes.foregroundColor) {
            color = (uint32_t)(*textAttributes.foregroundColor);
        }
        if (type == (int32_t)facebook::react::TextDecorationLineType::Strikethrough
            || type == (int32_t)facebook::react::TextDecorationLineType::UnderlineStrikethrough) {
            type = ARKUI_TEXT_DECORATION_TYPE_LINE_THROUGH;
        }
        spanNode->setTextDecoration(type, color);
    }

    // TextLineHeight
    float lineHeight =
        isnan(textAttributes.lineHeight) ? fontSize * (1 + DEFAULT_LINE_SPACING) : (float)textAttributes.lineHeight;
    VLOG(3) << "[text-debug] textAttributes.lineHeight=" << (float)textAttributes.lineHeight
            << ", lineHeight=" << lineHeight << ", index=" << index;
    spanNode->setTextLineHeight(lineHeight);

    // BackgroundColor
    VLOG(3) << "[text-debug] textAttributes.backgroundColor=" << (uint32_t)(*textAttributes.backgroundColor)
              << ", index=" << index;
    if (textAttributes.backgroundColor) {
        spanNode->setBackgroundStyle((uint32_t)(*textAttributes.backgroundColor));
    }

    // letterSpacing
    if (!std::isnan(textAttributes.letterSpacing)) {
        VLOG(3) << "[text-debug] textAttributes.letterSpacing=" << textAttributes.letterSpacing
                  << ", index=" << index;
        spanNode->setTextLetterSpacing(textAttributes.letterSpacing);
    }

    // shadow
    float textShadowRadius = isnan(textAttributes.textShadowRadius) ? 0.0f : textAttributes.textShadowRadius;
    int32_t textShadowType = ARKUI_SHADOW_TYPE_COLOR;
    uint32_t textShadowColor = *textAttributes.textShadowColor;
    float textShadowOffsetX =
        textAttributes.textShadowOffset.has_value() ? textAttributes.textShadowOffset.value().width : 0.0f;
    float textShadowOffsetY =
        textAttributes.textShadowOffset.has_value() ? textAttributes.textShadowOffset.value().height : 0.0f;
    VLOG(3) << "[text-debug] setTextShadow:" << textShadowRadius << ", textShadowType:" << textShadowType
              << ", textShadowColor:" << textShadowColor << ", textShadowOffsetX:" << textShadowOffsetX
              << ", textShadowOffsetY:" << textShadowOffsetY << ", index=" << index;
    spanNode->setTextShadow(textShadowRadius, textShadowType, textShadowColor, textShadowOffsetX, textShadowOffsetY);

    // text Case
    if (textAttributes.textTransform.has_value()) {
        VLOG(3) << "[text-debug] textAttributes.textTransform=" << (int)textAttributes.textTransform.value()
                  << ", index=" << index;
        switch (textAttributes.textTransform.value()) {
        case facebook::react::TextTransform::None:
        case facebook::react::TextTransform::Uppercase:
        case facebook::react::TextTransform::Lowercase: {
            int32_t textCase = TextConversions::getArkUITextCase(textAttributes.textTransform.value());
            spanNode->setTextCase(textCase);
            break;
        }
        case facebook::react::TextTransform::Capitalize: {
            spanNode->setSpanContent(stringCapitalize(fragment.string));
            break;
        }
        default:
            // do nothing
            break;
        }
    }
    VLOG(3) << "[text-debug] setFragment end, index=" << index;
}

void TextComponentInstance::setImageSpanSize(const facebook::react::Size &imageSize, std::shared_ptr<ImageSpanNode> imageSpanNode) {
    if (imageSpanNode == nullptr) {
        return;
    }
    imageSpanNode->setSize(imageSize);
}

void TextComponentInstance::setParagraphAttributes(const facebook::react::ParagraphAttributes &paragraphAttributes) {
    // TextMaxLines
    VLOG(3) << "[text-debug] paragraphAttributes.maximumNumberOfLines=" << paragraphAttributes.maximumNumberOfLines;
    if (paragraphAttributes.maximumNumberOfLines > 0) {
        m_textNode.setTextMaxLines(paragraphAttributes.maximumNumberOfLines);
    }

    // height Adaptive Policy
    VLOG(3) << "[text-debug] paragraphAttributes.heightAdaptivePolicy="
              << (int)paragraphAttributes.adjustsFontSizeToFit;
    int32_t heightAdaptivePolicy = paragraphAttributes.adjustsFontSizeToFit
                                       ? ARKUI_TEXT_HEIGHT_ADAPTIVE_POLICY_MIN_FONT_SIZE_FIRST
                                       : ARKUI_TEXT_HEIGHT_ADAPTIVE_POLICY_MAX_LINES_FIRST;
    m_textNode.setTextHeightAdaptivePolicy(heightAdaptivePolicy);

    // overflow
    int32_t ellipsizeMode = TextConversions::getArkUIEllipsizeMode(paragraphAttributes.ellipsizeMode);
    VLOG(3) << "[text-debug] paragraphAttributes.ellipsizeMode=" << ellipsizeMode;
    if (ellipsizeMode == -1) {
        m_textNode.setTextOverflow(ARKUI_TEXT_OVERFLOW_CLIP);
    } else {
        m_textNode.setTextOverflow(ARKUI_TEXT_OVERFLOW_ELLIPSIS);
        m_textNode.setTextEllipsisMode(ellipsizeMode);
    }
    return;
}

std::string TextComponentInstance::stringCapitalize(const std::string &strInput) {
    if (strInput == "") {
        return strInput;
    }

    std::string strRes = "";
    std::string split = " ";
    std::vector<std::string> subStringVector;
    subStringVector.clear();

    std::string strSrc = strInput + split;
    size_t pos = strSrc.find(split);
    int32_t step = split.size();

    while (pos != strSrc.npos) {
        std::string strTemp = strSrc.substr(0, pos);
        subStringVector.push_back(strTemp);

        strSrc = strSrc.substr(pos + step, strSrc.size());
        pos = strSrc.find(split);
    }

    for (auto subString : subStringVector) {
        if (std::isalpha(subString[0])) {
            std::transform(subString.begin(), subString.end(), subString.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            subString[0] = std::toupper(static_cast<unsigned char>(subString[0]));
        }
        if (!strRes.empty()) {
            strRes += split;
        }
        strRes += subString;
    }

    return strRes;
}

StackNode &TextComponentInstance::getLocalRootArkUINode() { return m_stackNode; }
