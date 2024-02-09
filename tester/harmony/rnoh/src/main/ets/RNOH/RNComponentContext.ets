import { RNOHContext } from "./RNOHContext"
import { Tag } from "./DescriptorBase"
import { RNInstanceImpl } from "./RNInstance"

export interface ComponentBuilderContext {
  /**
   * @deprecated Use `rnComponentContext` instead.
   */
  rnohContext: RNOHContext
  /**
   * @deprecated: Use tag and componentName instead
   */
  descriptor: DescriptorEssence
  tag: Tag
  componentName: string
  /**
   * rnComponentContext will replace rnohContext in the future
   */
  rnComponentContext: RNComponentContext
}

interface DescriptorEssence {
  tag: Tag,
  type: string
}

export type CustomComponentBuilder = (ctx: ComponentBuilderContext) => void

export type WrappedCustomRNComponentBuilder = WrappedBuilder<[ComponentBuilderContext]>

export type WrappedRNChildrenBuilder = WrappedBuilder<[RNComponentContext, Tag]>

export type WrappedRNComponentBuilder = WrappedBuilder<[RNComponentContext, Tag]>

export class RNComponentContext extends RNOHContext {
  public wrappedCustomRNComponentBuilder: WrappedCustomRNComponentBuilder
  public wrappedRNChildrenBuilder: WrappedRNChildrenBuilder
  public wrappedRNComponentBuilder: WrappedRNComponentBuilder

  constructor(
    rnohContext: RNOHContext,
    wrappedCustomRNComponentBuilder: WrappedCustomRNComponentBuilder,
    wrappedRNComponentBuilder: WrappedRNComponentBuilder
  ) {
    super(
      rnohContext.reactNativeVersion,
      rnohContext.rnInstance as RNInstanceImpl,
      rnohContext,
    )
    this.wrappedCustomRNComponentBuilder = wrappedCustomRNComponentBuilder
    this.wrappedRNChildrenBuilder = wrapBuilder(buildRNChildren)
    this.wrappedRNComponentBuilder = wrappedRNComponentBuilder
  }
}


/**
 * This component will be replaced with a builder in the future once usage of DataSource is possible.
 */
@Component
struct RNChildren {
  public ctx!: RNComponentContext
  public parentTag: Tag = 0
  @State childrenTags: Tag[] = []
  private cleanUpCallbacks: (() => void)[] = []

  aboutToAppear(): void {
    this.childrenTags = this.ctx.descriptorRegistry.getDescriptor(this.parentTag).childrenTags
    this.cleanUpCallbacks.push(this.ctx.descriptorRegistry.subscribeToDescriptorChanges(this.parentTag, (descriptor) => {
      this.childrenTags = descriptor.childrenTags
    }))
  }

  aboutToDisappear(): void {
    this.cleanUpCallbacks.forEach(cb => cb())
  }

  build() {
    Stack() {
      ForEach(this.childrenTags, (childrenTag: Tag) => {
        this.ctx.wrappedRNComponentBuilder.builder(this.ctx, childrenTag)
      }, (childrenTag: Tag) => childrenTag.toString())
    }
  }
}

@Builder
function buildRNChildren(ctx: RNComponentContext, parentTag: Tag) {
  RNChildren({ ctx, parentTag })
}
