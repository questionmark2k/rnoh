import type { TurboModuleContext } from '../../RNOH/TurboModule';
import { TurboModule } from '../../RNOH/TurboModule';
import { RNOHLogger } from '../../RNOH/RNOHLogger';
import { VibrationController } from '../../RNOH/VibrationController';

export class VibrationTurboModule extends TurboModule {
  public static readonly NAME = 'Vibration';

  private logger: RNOHLogger;
  private vibrationController: VibrationController

  constructor(protected ctx: TurboModuleContext) {
    super(ctx);
    this.logger = this.ctx.logger.clone("VibrationTurboModule")
    this.vibrationController = new VibrationController(this.logger)
  }

  public vibrate(pattern: number) {
    this.vibrationController.vibrate(pattern);
  };

  // Android only
  public vibrateByPattern(pattern: Array<number>, repeat: number) {
    this.logger.warn("Vibration::vibrateByPattern is not supported");
  };

  public cancel() {
    this.vibrationController.cancel();
  }
}