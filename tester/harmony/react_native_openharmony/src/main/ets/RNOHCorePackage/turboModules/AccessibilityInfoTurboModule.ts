import accessibility from '@ohos.accessibility';
import type { TurboModuleContext } from '../../RNOH/TurboModule';
import { TurboModule } from '../../RNOH/TurboModule';

export class AccessibilityInfoTurboModule extends TurboModule {
  public static readonly NAME = 'AccessibilityInfo';

  constructor(ctx: TurboModuleContext) {
    super(ctx);
  }

  public isTouchExplorationEnabled(
    resolveCallback: (isScreenReaderEnabled: boolean) => void,
  ) {
    const isScreenReaderEnabled = accessibility.isOpenTouchGuideSync();
    resolveCallback(isScreenReaderEnabled);
  }
}
