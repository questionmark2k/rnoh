import { RNComponentContext, RNOHContext, Tag, CustomComponentBuilder, DescriptorWrapper } from '../../RNOH'
import {
  ViewBaseDescriptor,
  ViewDescriptorWrapperBase,
  RNViewBaseAttributeModifierBase,
} from './RNViewBase'
import { RNComponentFactory } from "./RNComponentFactory"
import { RNViewManager } from '../componentManagers';

@Component
export struct RNView {
  public ctx!: RNOHContext
  public tag: number = 0
  @BuilderParam public buildCustomComponent: CustomComponentBuilder
  private componentManager!: RNViewManager;
  @State private childrenTags: Tag[] = []
  @State private descriptorWrapper: ViewDescriptorWrapperBase | undefined = undefined
  @State private rnViewAttributeModifier: RNViewBaseAttributeModifierBase | undefined = undefined
  private cleanUpCallbacks: (() => void)[] = []

  aboutToAppear() {
    this.componentManager = new RNViewManager(this.tag, this.ctx)
    this.cleanUpCallbacks.push(this.ctx.componentManagerRegistry.registerComponentManager(this.tag, this.componentManager))
    const descriptor = this.ctx.descriptorRegistry.getDescriptor<ViewBaseDescriptor>(this.tag)
    this.onDescriptorChange(descriptor)
    this.maybeLogStartUpTime()
    this.componentManager.setParentTag(descriptor.parentTag);
    this.cleanUpCallbacks.push(this.ctx.descriptorRegistry.subscribeToDescriptorChanges(this.tag,
      (newDescriptor) => {
        this.onDescriptorChange(newDescriptor)
      }
    ))
  }

  private onDescriptorChange(descriptor: ViewBaseDescriptor) {
    let descriptorWrapper = this.ctx.descriptorRegistry.findDescriptorWrapperByTag<ViewDescriptorWrapperBase>(descriptor.tag)
    if (!descriptorWrapper || !(descriptorWrapper instanceof ViewDescriptorWrapperBase)) {
      descriptorWrapper = new ViewDescriptorWrapperBase(descriptor)
    }
    if (this.ctx instanceof RNComponentContext) {
      this.rnViewAttributeModifier = new RNViewBaseAttributeModifierBase(this.ctx, descriptorWrapper)
    }
    this.descriptorWrapper = descriptorWrapper
    this.childrenTags = descriptorWrapper.childrenTags
  }

  private maybeLogStartUpTime() {
    if ((this.descriptorWrapper?.hints ?? []).includes("ready")) {
      this.ctx.markReadiness()
    }
  }

  aboutToDisappear() {
    this.cleanUpCallbacks.forEach(cb => cb())
  }

  onClickEvent(event: ClickEvent): void {
    if (event.source !== SourceType.TouchScreen) {
      this.ctx.rnInstance.emitComponentEvent(this.tag, "onClick", {})
    }
  }

  build() {
    Stack() {
      /**
       * Focus hack: container components cannot be focused if they don't contain focusable children.
       * An invisible button is added to a focusable View to avoid this problem.
       */
      if (this.descriptorWrapper?.focusable) {
        Button().width(0).height(0).padding(0).margin(0)
      }
      if (this.ctx instanceof RNComponentContext) {
        LazyForEach(this.ctx.createComponentDataSource({ tag: this.tag }),
          (descriptorWrapper: DescriptorWrapper) => {
            (this.ctx as RNComponentContext).wrappedRNComponentBuilder.builder((this.ctx as RNComponentContext), descriptorWrapper.tag)
          },
          (descriptorWrapper: DescriptorWrapper) => descriptorWrapper.tag.toString() + "@" + descriptorWrapper.renderKey
        )
      } else {
        ForEach(this.childrenTags, (childrenTag: Tag) => {
          RNComponentFactory({ ctx: this.ctx, tag: childrenTag, buildCustomComponent: this.buildCustomComponent })
        }, (childrenTag: Tag) => childrenTag.toString())
      }
    }
    .id(this.tag.toString())
    .onClick((event: ClickEvent) => this.onClickEvent(event))
    .attributeModifier(this.rnViewAttributeModifier)
    .align(Alignment.TopStart);
  }
}

function onClickEvent(event: ClickEvent, ctx: RNComponentContext, tag: Tag) {
  if (event.source !== SourceType.TouchScreen) {
    ctx.rnInstance.emitComponentEvent(tag, "onClick", {})
  }
}

interface RNViewStateHolder {
  componentManager?: RNViewManager
  onDisappearCallbacks?: (() => void)[]
}

@Builder
export function buildRNView(ctx: RNComponentContext, tag: Tag, stateHolder: RNViewStateHolder) {
  Stack() {
    /**
     * Focus hack: container components cannot be focused if they don't contain focusable children.
     * An invisible button is added to a focusable View to avoid this problem.
     */
    if ((ctx.descriptorRegistry.findDescriptorWrapperByTag(tag) as ViewDescriptorWrapperBase)?.focusable) {
      Button().width(0).height(0).padding(0).margin(0)
    }
    LazyForEach(ctx.createComponentDataSource({ tag }),
      (descriptorWrapper: DescriptorWrapper) => {
        ctx.wrappedRNComponentBuilder.builder(ctx, descriptorWrapper.tag)
      },
      (descriptorWrapper: DescriptorWrapper) => descriptorWrapper.tag.toString() + "@" + descriptorWrapper.renderKey
    )
  }
  .onAppear(() => {
    if (stateHolder.componentManager === undefined) {
      stateHolder.componentManager = new RNViewManager(tag, ctx);
      const cleanup = ctx.componentManagerRegistry.registerComponentManager(tag, stateHolder.componentManager);
      stateHolder.onDisappearCallbacks = [cleanup];
    }
  })
  .onDisAppear(() => {
    stateHolder.onDisappearCallbacks?.forEach(cb => cb());
    stateHolder.onDisappearCallbacks = undefined
  })
  .id(tag.toString())
  .onClick((event: ClickEvent) => onClickEvent(event, ctx, tag))
  .attributeModifier(new RNViewBaseAttributeModifierBase(ctx, ctx.descriptorRegistry.findDescriptorWrapperByTag(tag) as ViewDescriptorWrapperBase))
  .align(Alignment.TopStart)
}

