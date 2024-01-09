import { TurboModule } from "../../RNOH/TurboModule";

export class SourceCodeTurboModule extends TurboModule {
  public static readonly NAME = 'SourceCode';

  getConstants(): { scriptURL: string | null; } {
    return { scriptURL: this.ctx.rnInstance.getInitialBundleUrl() ?? '' };
  }
}