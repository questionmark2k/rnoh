import { DisplayMetrics } from './types';
import window from '@ohos.window';
import { RNOHLogger } from './RNOHLogger';
import display from '@ohos.display';
import { RNOHError } from "./RNOHError"

const defaultDisplayMetrics: DisplayMetrics = {
  windowPhysicalPixels: {
    width: 0,
    height: 0,
    scale: 1,
    fontScale: 1,
    densityDpi: 480,
  },
  screenPhysicalPixels: {
    width: 0,
    height: 0,
    scale: 1,
    fontScale: 1,
    densityDpi: 480
  },
} as const;

export class DisplayMetricsManager {
  private displayMetrics: DisplayMetrics = defaultDisplayMetrics;
  private logger: RNOHLogger

  constructor(logger: RNOHLogger) {
    this.logger = logger.clone("DisplayMetricsManager");
  }

  public updateWindowSize(windowSize: window.Size | window.Rect) {
    this.displayMetrics.windowPhysicalPixels.height = windowSize.height;
    this.displayMetrics.windowPhysicalPixels.width = windowSize.width;
    this.updateDisplayMetrics()
  }

  public updateDisplayMetrics() {
    try {
      const displayInstance = display.getDefaultDisplaySync();
      this.displayMetrics = {
        screenPhysicalPixels: {
          width: displayInstance.width,
          height: displayInstance.height,
          scale: displayInstance.densityPixels,
          fontScale: 1,
          densityDpi: displayInstance.densityDPI,
        },
        windowPhysicalPixels: {
          width: this.displayMetrics.windowPhysicalPixels.width,
          height: this.displayMetrics.windowPhysicalPixels.height,
          scale: displayInstance.densityPixels,
          fontScale: 1,
          densityDpi: displayInstance.densityDPI,
        }
      };
    }
    catch (err) {
      this.logger.error('Failed to update display size ' + JSON.stringify(err));
    }
  }

  public getDisplayMetrics(): DisplayMetrics {
    if (!this.displayMetrics) {
      throw new RNOHError({ whatHappened: "DisplayMetrics are undefined", howCanItBeFixed: [] });
    }
    return this.displayMetrics;
  }
}