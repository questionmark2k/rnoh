import { LifecycleState } from '../../RNOH/ts';
import type { TurboModuleContext } from "../../RNOH/TurboModule";
import { TurboModule } from "../../RNOH/TurboModule";

export class AppStateTurboModule extends TurboModule {
  public static readonly NAME = 'AppState';

  constructor(protected ctx: TurboModuleContext) {
    super(ctx);
    this.subscribeListeners();
  }

  private subscribeListeners() {
    this.ctx.rnInstance.subscribeToLifecycleEvents("FOREGROUND", () => {
      this.ctx.rnInstance.emitDeviceEvent("appStateDidChange", { app_state: this.getAppState() });
    })
    this.ctx.rnInstance.subscribeToLifecycleEvents("BACKGROUND", () => {
      this.ctx.rnInstance.emitDeviceEvent("appStateDidChange", { app_state: this.getAppState() });
    })
  }

  private getAppState() {
    const isActive = this.ctx.rnAbility.getAbilityState();

    return isActive === "FOREGROUND" ? 'active' : 'background';
  }

  getConstants() {
    return { initialAppState: this.getAppState() };
  }

  getCurrentAppState(success: (appState: { app_state: string }) => void, error: (error: Error) => void) {
    success({ app_state: this.getAppState() });
  };
}

