import {
  AnyJSBundleProvider,
  MetroJSBundleProvider,
  ResourceJSBundleProvider,
  RNInstance,
  RNOHCoreContext,
  RNOHLogger,
  SurfaceHandle,
  Tag
} from 'rnoh/ts';
import { createRNPackages } from '../RNPackagesFactory';
import window from '@ohos.window';
import { RNAbility } from 'rnoh';

export default class EntryAbility extends RNAbility {
  public async onWindowSetup(windowStage: window.WindowStage, providedLogger: RNOHLogger): Promise<void> {
    super.onWindowSetup(windowStage, providedLogger)
    const rnohCoreContext = AppStorage.get<RNOHCoreContext>('RNOHCoreContext')!;
    const rnInstance: RNInstance = await rnohCoreContext.createAndRegisterRNInstance({
      createRNPackages,
      enableCAPIArchitecture: true
    }, {
      create(tag: Tag, componentName: string): undefined {
        return undefined;
      }
    })
    rnInstance.runJSBundle(new AnyJSBundleProvider([new MetroJSBundleProvider(), new ResourceJSBundleProvider(this.context.resourceManager, 'bundle.harmony.js')]))
    AppStorage.setOrCreate("AppRNInstance", rnInstance);

    const surfaceByAppKey: Record<string, SurfaceHandle> = {};
    AppStorage.setOrCreate("ReactSurfaceByAppKey", surfaceByAppKey);
  }

  getPagePath() {
    return 'pages/Index';
  }
}
