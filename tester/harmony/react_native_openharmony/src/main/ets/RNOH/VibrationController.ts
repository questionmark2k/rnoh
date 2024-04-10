import vibrator from '@ohos.vibrator'
import { RNOHLogger } from "./RNOHLogger"

export class VibrationController {
  private logger: RNOHLogger

  constructor(logger: RNOHLogger) {
    this.logger = logger.clone("Vibration")
  }

  public vibrate(pattern: number) {
    vibrator.startVibration({
        type: 'time',
        duration: pattern,
      }, {
        id: 0,
        usage: 'simulateReality'
      }, (error) => {
        if (error) {
          this.logger.error('RNOH Vibration::vibrate error', error);
          return;
        }
      });
  }

  public cancel() {
    vibrator.stopVibration();
  }
}