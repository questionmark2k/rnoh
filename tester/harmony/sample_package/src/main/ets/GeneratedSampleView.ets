import { RNComponentContext, RNViewBase } from 'rnoh';
import { RNC } from "rnoh/generated"
import { getDirectProps } from "./GeneratedSampleViewUtils"

@Component
export struct GeneratedSampleView {
  public static readonly NAME = RNC.GeneratedSampleView.NAME
  public ctx!: RNComponentContext
  public tag: number = 0
  @State private descriptorWrapper: RNC.GeneratedSampleView.DescriptorWrapper = {
  } as RNC.GeneratedSampleView.DescriptorWrapper
  private eventEmitter: RNC.GeneratedSampleView.EventEmitter | undefined = undefined
  private cleanUpCallbacks: (() => void)[] = []

  aboutToAppear() {
    this.eventEmitter = new RNC.GeneratedSampleView.EventEmitter(this.ctx.rnInstance, this.tag)
    this.onDescriptorWrapperChange(this.ctx.descriptorRegistry.findDescriptorWrapperByTag<RNC.GeneratedSampleView.DescriptorWrapper>(this.tag)!)
    this.cleanUpCallbacks.push(this.ctx.descriptorRegistry.subscribeToDescriptorChanges(this.tag,
      (_descriptor, newDescriptorWrapper) => {
        this.onDescriptorWrapperChange(newDescriptorWrapper! as RNC.GeneratedSampleView.DescriptorWrapper)
      }
    ))

    this.cleanUpCallbacks.push(new RNC.GeneratedSampleView.CommandReceiver(this.ctx.componentCommandReceiver, this.tag).subscribe("emitNativeEvent", (argv) => {
      const eventType = argv[0] as keyof RNC.GeneratedSampleView.EventPayloadByName
      this.eventEmitter!.emit(eventType, this.getDirectProps())
    }))
  }

  private onDescriptorWrapperChange(descriptorWrapper: RNC.GeneratedSampleView.DescriptorWrapper) {
    this.descriptorWrapper = descriptorWrapper
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

      this.ctx.wrappedRNChildrenBuilder.builder(this.ctx, this.tag)
    }
  }
}
