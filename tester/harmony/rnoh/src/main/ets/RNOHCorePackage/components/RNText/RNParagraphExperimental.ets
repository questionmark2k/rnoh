import {
  RNComponentContext,
  RNOHContext,
  convertColorSegmentsToString,
  Tag,
  CustomComponentBuilder,
  DEFAULT_LINE_SPACING,
  DescriptorWrapper
} from '../../../RNOH'
import { RNViewBase } from '../RNViewBase'
import { PLACEHOLDER_SYMBOL } from '../../../ParagraphMeasurer';
import { PositionedLine, Size, } from '../../../ParagraphMeasurer'
import { RNParagraphManager } from './RNParagraphManager'
import { RNComponentFactory } from '../RNComponentFactory'
import { TextDescriptor, TextFragmentExtraData, AttributedFragment } from './types'
import { ViewDescriptorWrapper } from "../RNViewBase"
import {
  getLetterSpacing,
  getTextDecorationLine,
  getTextShadow,
  getMaxLines,
  stringToAlignment,
  stringToVerticalAlign,
  ellipsizeModeToTextOverflow,
  stringToStackAlignment,
  stringToFontStyle,
  getFontSize,
  stringToTextTransform,
} from './conversions'

@Component
export struct RNParagraphExperimental {
  public ctx!: RNOHContext
  public tag: number = 0
  /** @deprecated  */
  @BuilderParam public buildCustomComponent: CustomComponentBuilder
  //--------------------------------------------------------------------------------------------------------------------
  @State descriptor: TextDescriptor = {} as TextDescriptor
  @State positionedLines: PositionedLine<TextFragmentExtraData>[] = []
  @State paragraphSize: Size = { width: 0, height: 0 }
  @State descriptorWrapper: ViewDescriptorWrapper | undefined = undefined
  private componentManager!: RNParagraphManager
  private cleanUpCallbacks: (() => void)[] = []

  aboutToAppear() {
    this.onDescriptorChange(this.ctx.descriptorRegistry.getDescriptor<TextDescriptor>(this.tag))
    this.cleanUpCallbacks.push(this.ctx.descriptorRegistry.subscribeToDescriptorChanges(this.tag,
      (newDescriptor) => {
        this.onDescriptorChange(newDescriptor as TextDescriptor)
      }
    ))
  }

  aboutToDisappear() {
    this.cleanUpCallbacks.forEach((cb) => cb())
  }

  onDescriptorChange(newDescriptor: TextDescriptor) {
    this.descriptor = newDescriptor
    this.descriptorWrapper = new ViewDescriptorWrapper(newDescriptor)
  }

  getVerticalAlignmentOffset() {
    const maxOffset = Math.max(this.descriptor.layoutMetrics.frame.size.height - this.paragraphSize.height, 0)
    switch (this.descriptor.props.textAlignVertical) {
      case "bottom":
        return maxOffset
      case "center":
        return maxOffset / 2
      default:
        return 0
    }
  }

  build() {
    RNViewBase({
      ctx: this.ctx,
      tag: this.tag,
      componentManager: this.componentManager,
    }) {
      Text(){
        ForEach(this.descriptor.props.fragments, (fragment:AttributedFragment) => {
          if (fragment.text !== PLACEHOLDER_SYMBOL) {
            Span(fragment.text)
              .fontColor(convertColorSegmentsToString(fragment.fontColor))
              .fontSize(getFontSize(fragment.fontSize, this.descriptor.rawProps))
              .fontFamily(fragment.fontFamily)
              .fontWeight(fragment.fontWeight)
              .fontStyle(stringToFontStyle(fragment.fontStyle))
              .decoration({
                type: getTextDecorationLine(fragment.textDecorationLine),
                color: fragment.textDecorationColor
                  ? convertColorSegmentsToString(fragment.textDecorationColor)
                  : convertColorSegmentsToString(fragment.fontColor)
              })
              .letterSpacing(getLetterSpacing(fragment.letterSpacing))
              .lineHeight(fragment.lineHeight)
              .textShadow(getTextShadow(fragment.textShadowProps))
                // somehow backgroundColor doesn't work, set it using textBackgroundStyle
              .textBackgroundStyle({ color:convertColorSegmentsToString(fragment.backgroundColor) })
              .textCase(stringToTextTransform(fragment.textTransform))
          } else {
            ImageSpan('')
              .width(fragment.parentShadowView?.layoutMetrics.frame.size.width)
              .height(fragment.parentShadowView?.layoutMetrics.frame.size.height)
          }
        })
      }
      // setting paragraph attributes to the container text component
      .textAlign(stringToAlignment(this.descriptor.props.textAlign))
      .width(this.descriptor.layoutMetrics.frame.size.width)
      .height(this.descriptor.layoutMetrics.frame.size.height)
      .align(stringToVerticalAlign(this.descriptor.props.textAlignVertical as string))
      .padding(this.descriptor.props.padding)
      .maxLines(getMaxLines(this.descriptor.props.maximumNumberOfLines))
      .copyOption(this.descriptor.props.selectable ? CopyOptions.LocalDevice : CopyOptions.None)
      .enabled(!this.descriptor.props.disabled)
      .textOverflow({ overflow: ellipsizeModeToTextOverflow(this.descriptor.props.ellipsizeMode) })
      if (this.ctx instanceof RNComponentContext) {
        LazyForEach(this.ctx.createComponentDataSource({ tag: this.tag }),
          (descriptorWrapper: DescriptorWrapper) => {
            (this.ctx as RNComponentContext).wrappedRNComponentBuilder.builder((this.ctx as RNComponentContext), descriptorWrapper.tag)
          },
          (descriptorWrapper: DescriptorWrapper) => descriptorWrapper.tag.toString() + "@" + descriptorWrapper.renderKey
        )
      } else {
        ForEach(this.descriptor.childrenTags, (childrenTag: Tag) => {
          RNComponentFactory({ ctx: this.ctx, tag: childrenTag, buildCustomComponent: this.buildCustomComponent })
        }, (childrenTag: Tag) => childrenTag.toString())
      }
    }
  }
}
