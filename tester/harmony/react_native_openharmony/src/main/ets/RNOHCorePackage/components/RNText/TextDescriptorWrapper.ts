import { Descriptor } from '../../../RNOH/DescriptorBase'
import {
  EllipsizeMode,
  PaddingProps,
  ParagraphState,
  TextProps,
  TextRawProps,
  ViewDescriptorWrapperBase,
  ViewPropsSelector
} from '../ts'

export class TextDescriptorWrapper extends ViewDescriptorWrapperBase<string,
TextProps,
ParagraphState,
TextRawProps> {
  constructor(descriptor: Descriptor<string, TextProps, ParagraphState, TextRawProps>) {
    super(descriptor)
  }

  public get selectable(): boolean {
    return this.descriptor.rawProps.selectable
  }

  public get enabled(): boolean {
    return !this.rawProps.disabled;
  }

  public get textAlign(): string | undefined {
    if (!('textAlign' in this.cppProps)) {
      return undefined;
    }
    return this.cppProps.textAlign;
  }

  public get textAlignVertical(): string | undefined {
    if (!('textAlignVertical' in this.cppProps)) {
      return undefined;
    }
    return this.cppProps.textAlignVertical;
  }

  public get maximumNumberOfLines(): number {
    if (!('maximumNumberOfLines' in this.cppProps)) {
      return undefined;
    }
    return this.cppProps.maximumNumberOfLines;
  }

  public get ellipsizeMode(): EllipsizeMode {
    if (!('ellipsizeMode' in this.cppProps)) {
      return undefined;
    }
    return this.cppProps.ellipsizeMode;
  }

  public get paragraphPadding(): PaddingProps {
    if (!('padding' in this.cppProps)) {
      return undefined;
    }
    return this.cppProps.padding
  }
}