import type { TurboModuleContext, SafeAreaInsets } from "../../RNOH/ts";
import { TurboModule } from "../../RNOH/ts";
import { StatusBarTurboModule } from "./StatusBarTurboModule"


export interface StatusBarStatusProvider {
  isStatusBarHidden(): boolean
}

export class SafeAreaTurboModule extends TurboModule {
  public static readonly NAME = 'SafeAreaTurboModule';

  private initialInsets: SafeAreaInsets

  static async create(ctx: TurboModuleContext, statusBarTurboModule: StatusBarTurboModule) {
    return new SafeAreaTurboModule(ctx, statusBarTurboModule)
  }

  constructor(ctx: TurboModuleContext, statusBarTurboModule: StatusBarTurboModule) {
    super(ctx)
    this.initialInsets = ctx.safeAreaInsetsProvider.safeAreaInsets;
    ctx.safeAreaInsetsProvider.eventEmitter.subscribe("SAFE_AREA_INSETS_CHANGE", this.onSafeAreaChange.bind(this))
    // Hiding/Showing StatusBar is reflected immediately in SafeAreaView
    statusBarTurboModule.subscribe("SYSTEM_BAR_VISIBILITY_CHANGE", () => ctx.safeAreaInsetsProvider.onSafeAreaChange())
  }

  private onSafeAreaChange(insets: SafeAreaInsets) {
    this.ctx.rnInstance.emitDeviceEvent("SAFE_AREA_INSETS_CHANGE", insets);
  }

  public getInitialInsets(): SafeAreaInsets {
    return this.initialInsets
  }
}
