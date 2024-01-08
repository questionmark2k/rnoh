import WindowUtils from '@ohos.window';
import type { TurboModuleContext, SafeAreaInsets } from "../../RNOH/ts";
import { TurboModule, createSafeAreaInsets } from "../../RNOH/ts";
import { StatusBarTurboModule } from "./StatusBarTurboModule"


export interface StatusBarStatusProvider {
  isStatusBarHidden(): boolean
}

export class SafeAreaTurboModule extends TurboModule {
  public static readonly NAME = 'SafeAreaTurboModule';

  static async create(ctx: TurboModuleContext, statusBarTurboModule: StatusBarTurboModule) {
    const initialInsets = await createSafeAreaInsets(ctx.uiAbilityContext, statusBarTurboModule.isStatusBarHidden())
    const window = await WindowUtils.getLastWindow(ctx.uiAbilityContext)
    return new SafeAreaTurboModule(ctx, initialInsets, window, statusBarTurboModule)
  }


  constructor(ctx: TurboModuleContext, private initialInsets: SafeAreaInsets, private window: WindowUtils.Window, statusBarTurboModule: StatusBarTurboModule) {
    super(ctx)
    window.on("avoidAreaChange", this.onSafeAreaChange.bind(this))
    // Hiding/Showing StatusBar is reflected immediately in SafeAreaView
    statusBarTurboModule.subscribe("SYSTEM_BAR_VISIBILITY_CHANGE", this.onSafeAreaChange.bind(this))
  }

  private onSafeAreaChange() {
    createSafeAreaInsets(this.ctx.uiAbilityContext, this.ctx.rnInstance.getTurboModule<StatusBarTurboModule>(StatusBarTurboModule.NAME).isStatusBarHidden()).then((insets) => {
      this.ctx.rnInstance.emitDeviceEvent("SAFE_AREA_INSETS_CHANGE", insets);
    })
  }

  public getInitialInsets(): SafeAreaInsets {
    return this.initialInsets
  }
}



