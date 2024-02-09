import { RNInstance, RNComponentContext, CustomComponentBuilder } from "./RNOH"
import { RNSurface, SurfaceConfig2 } from "./RNSurface"


@CustomDialog
export struct LogBoxDialog {
  public ctx!: RNComponentContext
  public controller: CustomDialogController
  public rnInstance!: RNInstance
  public initialProps: Record<string, string> = {}
  @BuilderParam public buildCustomComponent!: CustomComponentBuilder

  build() {
    RNSurface({
      ctx: this.ctx!,
      surfaceConfig: {
        initialProps: this.initialProps ?? {},
        appKey: "LogBox",
      } as SurfaceConfig2,
      buildCustomComponent: this.buildCustomComponent,
    })
  }
}
