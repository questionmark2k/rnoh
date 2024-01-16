import { AnyJSBundleProvider, MetroJSBundleProvider, ResourceJSBundleProvider, RNAbility, SurfaceHandle} from 'rnoh/ts';
import { createRNPackages } from '../RNPackagesFactory';
import window from '@ohos.window';

export default class EntryAbility extends RNAbility {
  public async onWindowSetup(win: window.Window): Promise<void> {
    const rnInstance = await this.createAndRegisterRNInstance({createRNPackages})
    rnInstance.runJSBundle(new AnyJSBundleProvider([new MetroJSBundleProvider(), new ResourceJSBundleProvider(this.context.resourceManager, 'bundle.harmony.js')]))
    AppStorage.setOrCreate("AppRNInstance", rnInstance);

    const surfaceByAppKey: Record<string, SurfaceHandle> = {};
    AppStorage.setOrCreate("ReactSurfaceByAppKey", surfaceByAppKey);
  }

  getPagePath() {
    return 'pages/Index';
  }
}
