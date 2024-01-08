import { RNAbility } from "./RNAbility"
import { SafeAreaInsets, createSafeAreaInsets } from "./SafeAreaInsets"

@Component
export struct RNDevLoadingView {
  public useSafeAreaInsets: boolean = false
  @StorageLink('RNAbility') private rnAbility: RNAbility = {} as RNAbility
  private cleanUpCallbacks: (() => void)[] = []
  @State private message: string = ""
  @State private safeAreaInsets: SafeAreaInsets | undefined = undefined

  aboutToAppear() {
    createSafeAreaInsets(this.rnAbility.context, false).then(safeAreaInsets => this.safeAreaInsets = safeAreaInsets)
    this.cleanUpCallbacks.push(this.rnAbility.devToolsController.eventEmitter.subscribe("SHOW_DEV_LOADING_VIEW", (_rnInstanceId, msg) => {
      this.message = msg
    }))
    this.cleanUpCallbacks.push(this.rnAbility.devToolsController.eventEmitter.subscribe("HIDE_DEV_LOADING_VIEW", () => {
      this.message = ""
    }))
  }

  aboutToDisappear(): void {
    this.cleanUpCallbacks.forEach(cb => cb())
  }

  build() {
    if (this.safeAreaInsets && this.message) {
      Text(this.message)
        .fontColor("white")
        .fontSize(12)
        .backgroundColor("rgba(0,0,0,0.6)")
        .width("100%")
        .padding({ top: 8, bottom: 8 })
        .position({ x: 0, y: this.useSafeAreaInsets ? this.safeAreaInsets.top : 0 })
        .textAlign(TextAlign.Center)
    }

  }
}
