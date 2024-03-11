import { TurboModule, TurboModuleContext } from '../../RNOH/TurboModule';
import resourceManager from '@ohos.resourceManager';
import { convertColorValueToRGBA } from '../../RNOH/CppBridgeUtils';

export class PlatformColorTurboModule extends TurboModule {
  public static readonly NAME = 'PlatformColor';

  constructor(ctx: TurboModuleContext) {
    super(ctx);
  }

  public getSystemColor(colorKeys: string[]) {
    for (let colorKey of colorKeys) {
      const systemColor = this.getColorFromResources(colorKey);
      if (systemColor) {
        return systemColor;
      }
    }

    return null;
  }

  private getColorFromResources(colorKey: string) {
    let systemColor = null;

    // under the hood getColorByNameSync will throw exception if colorKey doesn't exist
    try {
      systemColor = resourceManager.getSystemResourceManager().getColorByNameSync(`${colorKey}`);
    } catch (err) {
    }

    return systemColor ? convertColorValueToRGBA(systemColor) : systemColor;
  }
}
