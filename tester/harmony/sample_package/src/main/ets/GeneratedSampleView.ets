import { ComponentBuilderContext, Tag, RNComponentFactory, RNOHContext, RNViewBase, } from 'rnoh';
import { RNC } from "rnoh/generated"
import { getDirectProps } from "./GeneratedSampleViewUtils"


@Component
export struct GeneratedSampleView {
  static readonly NAME = RNC.GeneratedSampleView.NAME
  ctx!: RNOHContext
  tag: number = 0
  @BuilderParam buildCustomComponent: (componentBuilderContext: ComponentBuilderContext) => void
  @State descriptorWrapper: RNC.GeneratedSampleView.DescriptorWrapper = {} as RNC.GeneratedSampleView.DescriptorWrapper
  @State fontSize: number | undefined = undefined
  private eventEmitter: RNC.GeneratedSampleView.EventEmitter | undefined = undefined
  private cleanUpCallbacks: (() => void)[] = []

  aboutToAppear() {
    this.eventEmitter = new RNC.GeneratedSampleView.EventEmitter(this.ctx.rnInstance, this.tag)
    this.onDescriptorChange(this.ctx.descriptorRegistry.getDescriptor<RNC.GeneratedSampleView.Descriptor>(this.tag))
    this.cleanUpCallbacks.push(this.ctx.descriptorRegistry.subscribeToDescriptorChanges(this.tag,
      (newDescriptor) => {
        this.onDescriptorChange(newDescriptor as RNC.GeneratedSampleView.Descriptor)
      }
    ))

    this.cleanUpCallbacks.push(new RNC.GeneratedSampleView.CommandReceiver(this.ctx.componentCommandReceiver, this.tag).subscribe("emitNativeEvent", (argv) => {
      const eventType = argv[0] as keyof RNC.GeneratedSampleView.EventPayloadByName
      this.eventEmitter!.emit(eventType, this.getDirectProps())
    }))
  }

  private onDescriptorChange(descriptor: RNC.GeneratedSampleView.Descriptor) {
    this.descriptorWrapper = new RNC.GeneratedSampleView.DescriptorWrapper(descriptor)
  }

  private getDirectProps() {
    return getDirectProps(this.descriptorWrapper)
  }

  aboutToDisappear() {
    this.cleanUpCallbacks.forEach(cb => cb())
  }

  build() {
    RNViewBase({ ctx: this.ctx, tag: this.tag }) {
      Column() {
        Text(JSON.stringify(this.getDirectProps(), null, 2)).width("100%").fontSize(8)
      }.width("100%").height("100%").backgroundColor(this.descriptorWrapper.props.colorTest.toRGBAString())

      ForEach(this.descriptorWrapper.childrenTags, (tag: Tag) => {
        RNComponentFactory({ ctx: this.ctx, tag: tag, buildCustomComponent: this.buildCustomComponent })
      }, (tag: Tag) => tag.toString())
    }
  }
}
