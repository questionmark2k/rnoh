import { TurboModule } from "../../RNOH/ts"

export class DevLoadingViewTurboModule extends TurboModule {
  static readonly NAME = "DevLoadingView"

  public showMessage(message: string, withColor?: number, withBackgroundColor?: number) {
    this.ctx.devToolsController.eventEmitter.emit("SHOW_DEV_LOADING_VIEW", this.ctx.rnInstance.getId(), message, withColor, withBackgroundColor)
  };

  public hide() {
    this.ctx.devToolsController.eventEmitter.emit("HIDE_DEV_LOADING_VIEW", this.ctx.rnInstance.getId())
  };
}