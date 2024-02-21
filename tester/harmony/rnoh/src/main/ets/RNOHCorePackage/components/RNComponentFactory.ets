import {
  Tag,
  RNOHContext,
  RNComponentContext,
  DescriptorWrapper,
  CustomComponentBuilder,
  ComponentBuilderContext
} from "../../RNOH"
import { buildRNView, RNView } from "./RNView"
import { RNImage } from "./RNImage"
import { RNTextInput } from "./RNTextInput"
import { RNText } from "./RNText"
import { RNScrollView } from "./RNScrollView"
import { Descriptor } from "../../RNOH/DescriptorBase";
import { RNModalHostView } from './RNModalHostView'
import { RNSwitch } from './RNSwitch'
import { RNActivityIndicator } from './RNActivityIndicator'

/** @deprecated */
interface DescriptorEssence {
  tag: Tag,
  type: string
}


class ComponentBuilderContextImpl implements ComponentBuilderContext {
  public rnComponentContext: RNComponentContext
  /** @deprecated: use rnComponentContext instead  */
  public rnohContext: RNOHContext
  public tag: Tag
  public componentName: string
  public descriptor: DescriptorEssence

  constructor(rnComponentContext: RNComponentContext, rnohContext: RNOHContext, tag: Tag, componentName: string, descriptor: DescriptorEssence) {
    this.rnComponentContext = rnComponentContext
    this.rnohContext = rnohContext
    this.tag = tag
    this.componentName = componentName
    this.descriptor = descriptor
  }
}


@Builder
export function buildRNComponent(ctx: RNComponentContext, descriptorWrapper: DescriptorWrapper) {
  if (ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) === "View" || ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) === "RootView") {
    buildRNView(ctx, descriptorWrapper.tag, {})
  } else if (ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) === "Image") {
    RNImage({
      ctx: ctx,
      tag: descriptorWrapper.tag,
    })
  } else if (ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) === "TextInput") {
    RNTextInput({
      ctx: ctx,
      tag: descriptorWrapper.tag,
    })
  } else if (ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) === "Paragraph") {
    RNText({
      ctx: ctx,
      tag: descriptorWrapper.tag,
    })
  } else if (ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) === "PullToRefreshView") {
    /**
     * RNScrollView displays Refresh component if PullToRefreshView is its direct parent.
     * Native Refresh component works correctly, only when a native Scroll is its direct child.
     */
    buildRNComponent(ctx, ctx.descriptorRegistry.findDescriptorWrapperByTag(descriptorWrapper.childrenTags[0])!)
  }
  else if (ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) === "ScrollView") {
    RNScrollView({
      ctx: ctx,
      tag: descriptorWrapper.tag,
    })
  } else if (ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) === "Switch") {
    RNSwitch({
      ctx: ctx,
      tag: descriptorWrapper.tag,
    })
  } else if (ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) === "ModalHostView") {
    RNModalHostView({
      ctx: ctx,
      tag: descriptorWrapper.tag,
    })
  } else if (ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) === "ActivityIndicatorView") {
    RNActivityIndicator({
      ctx: ctx,
      tag: descriptorWrapper.tag,
    })
  } else if (ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_) !== '') {
    ctx.wrappedCustomRNComponentBuilder.builder(new ComponentBuilderContextImpl(
      ctx,
      ctx,
      descriptorWrapper.tag,
      ctx.rnInstance.getComponentNameFromDescriptorType(descriptorWrapper.type_),
      ctx.descriptorRegistry.getDescriptor<Descriptor>(descriptorWrapper.tag) as DescriptorEssence,
    ))
  }
}

/**
 * @deprecated: Use one of  builder wrappers in `RNComponentContext` instead.
 */
@Component
export struct RNComponentFactory {
  public ctx!: RNOHContext
  public tag: number = 0
  @BuilderParam public buildCustomComponent!: CustomComponentBuilder
  @State private componentName: string = ''
  @State private childrenTags: Tag[] = []
  private cleanupCallback?: () => void = undefined

  aboutToAppear() {
    const descriptor = this.ctx.descriptorRegistry.getDescriptor<Descriptor>(this.tag)
    if (descriptor) {
      this.setDescriptor(descriptor)
    }
    this.cleanupCallback = this.ctx.descriptorRegistry.subscribeToDescriptorChanges(
      this.tag,
      (newDescriptor) => this.setDescriptor(newDescriptor)
    )
  }

  aboutToDisappear() {
    this.cleanupCallback?.()
  }

  setDescriptor(newDescriptor: Descriptor) {
    this.componentName = this.ctx.rnInstance.getComponentNameFromDescriptorType(newDescriptor.type)
    this.childrenTags = newDescriptor.childrenTags
  }

  build() {
    if (this.componentName === "View" || this.componentName === "RootView") {
      RNView({
        ctx: this.ctx,
        tag: this.tag,
        buildCustomComponent: this.buildCustomComponent,
      })
    } else if (this.componentName === "Image") {
      RNImage({ ctx: this.ctx, tag: this.tag })
    } else if (this.componentName === "TextInput") {
      RNTextInput({ ctx: this.ctx, tag: this.tag })
    } else if (this.componentName === "Paragraph") {
      RNText({ ctx: this.ctx, tag: this.tag, buildCustomComponent: this.buildCustomComponent })
    } else if (this.componentName === "PullToRefreshView") {
      /**
       * RNScrollView displays Refresh component if PullToRefreshView is its direct parent.
       * Native Refresh component works correctly, only when a native Scroll is its direct child.
       */
      RNComponentFactory({
        ctx: this.ctx,
        tag: this.childrenTags[0],
        buildCustomComponent: this.buildCustomComponent
      })
    }
    else if (this.componentName === "ScrollView") {
      RNScrollView({
        ctx: this.ctx,
        tag: this.tag,
        buildCustomComponent: this.buildCustomComponent
      })
    } else if (this.componentName === "Switch") {
      RNSwitch({
        ctx: this.ctx,
        tag: this.tag,
      })
    } else if (this.componentName === "ModalHostView") {
      RNModalHostView({
        ctx: this.ctx,
        tag: this.tag,
        buildCustomComponent: this.buildCustomComponent
      })
    } else if (this.componentName === "ActivityIndicatorView") {
      RNActivityIndicator({
        ctx: this.ctx,
        tag: this.tag,
      })
    } else if (this.componentName !== '' && this.ctx instanceof RNComponentContext) {
      this.buildCustomComponent(new ComponentBuilderContextImpl(
        this.ctx,
        this.ctx,
        this.tag,
        this.componentName,
        this.ctx.descriptorRegistry.getDescriptor<Descriptor>(this.tag) as DescriptorEssence,
      ))
    }
  }
}