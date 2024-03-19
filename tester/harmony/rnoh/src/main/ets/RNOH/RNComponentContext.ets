import { RNOHContext } from "./RNOHContext"
import { DescriptorWrapper, Tag } from "./DescriptorBase"
import { RNInstanceImpl } from "./RNInstance"
import { RNComponentDataSource } from './RNComponentDataSource'

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

export interface RNComponentDataSourceFactoryContext {
  tag: Tag
}

export type RNComponentDataSourceFactory = (ctx: RNComponentDataSourceFactoryContext) => RNComponentDataSource

export class RNComponentContext extends RNOHContext {
  public wrappedCustomRNComponentBuilder: WrappedCustomRNComponentBuilder
  public wrappedRNChildrenBuilder: WrappedRNChildrenBuilder
  public wrappedRNComponentBuilder: WrappedRNComponentBuilder
  private dataSourceFactoriesByType: Map<string, RNComponentDataSourceFactory>

  public createComponentDataSource(ctx: RNComponentDataSourceFactoryContext) {
    const descriptorWrapper = this.descriptorRegistry.findDescriptorWrapperByTag(ctx.tag);
    const createComponentDataSource = descriptorWrapper && this.dataSourceFactoriesByType.get(descriptorWrapper.type_);
    return createComponentDataSource ? createComponentDataSource(ctx) : new RNComponentDataSource(this, ctx.tag);
  }

  constructor(
    rnohContext: RNOHContext,
    wrappedCustomRNComponentBuilder: WrappedCustomRNComponentBuilder,
    wrappedRNComponentBuilder: WrappedRNComponentBuilder,
    rnComponentDataSourceFactoriesByDescriptorType: Map<string, RNComponentDataSourceFactory>
  ) {
    super(
      rnohContext.reactNativeVersion,
      rnohContext.rnInstance as RNInstanceImpl,
      rnohContext,
    )
    this.wrappedCustomRNComponentBuilder = wrappedCustomRNComponentBuilder
    this.wrappedRNChildrenBuilder = wrapBuilder(buildRNChildren)
    this.wrappedRNComponentBuilder = wrappedRNComponentBuilder
    this.dataSourceFactoriesByType = rnComponentDataSourceFactoriesByDescriptorType
  }
}

@Builder
function buildRNChildren(ctx: RNComponentContext, parentTag: Tag) {
  Stack() {
    LazyForEach(ctx.createComponentDataSource({ tag: parentTag }),
      (descriptorWrapper: DescriptorWrapper) => {
        ctx.wrappedRNComponentBuilder.builder(ctx, descriptorWrapper.tag)
      },
      (descriptorWrapper: DescriptorWrapper) => descriptorWrapper.tag.toString() + "@" + descriptorWrapper.renderKey
    )
  }
}

export class ComponentBuilderContextImpl implements ComponentBuilderContext {
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