import type { TurboModuleContext } from '../../RNOH/TurboModule';
import { TurboModule } from '../../RNOH/TurboModule';
import { RNOHLogger } from '../../RNOH/RNOHLogger';

export class VibrationTurboModule extends TurboModule {
  public static readonly NAME = 'Vibration';

  private logger: RNOHLogger;

  constructor(protected ctx: TurboModuleContext) {
    super(ctx);
    this.logger = this.ctx.logger.clone("VibrationTurboModule")
  }

  public vibrate(pattern: number) {
    this.ctx.rnAbility.vibration.vibrate(pattern);
  };

  // Android only
  public vibrateByPattern(pattern: Array<number>, repeat: number) {
    this.logger.warn("Vibration::vibrateByPattern is not supported");
  };

  public cancel() {
    this.ctx.rnAbility.vibration.cancel();
  }
}