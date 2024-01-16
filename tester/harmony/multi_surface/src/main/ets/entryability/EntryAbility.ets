import { AnyJSBundleProvider, MetroJSBundleProvider, ResourceJSBundleProvider, RNAbility, SurfaceHandle} from 'rnoh/ts';
import { createRNPackages } from '../RNPackagesFactory';
import AbilityConstant from '@ohos.app.ability.AbilityConstant';
import Want from '@ohos.app.ability.Want';

export default class EntryAbility extends RNAbility {
  async onCreate(want: Want, launchParam: AbilityConstant.LaunchParam) {
    super.onCreate(want, launchParam);

    const rnInstance = await this.createAndRegisterRNInstance({createRNPackages});
    rnInstance.runJSBundle(new AnyJSBundleProvider([new MetroJSBundleProvider(), new ResourceJSBundleProvider(this.context.resourceManager, 'bundle.harmony.js')]))
    AppStorage.setOrCreate("AppRNInstance", rnInstance);

    const surfaceByAppKey: Record<string, SurfaceHandle> = {};
    AppStorage.setOrCreate("ReactSurfaceByAppKey", surfaceByAppKey);
  }

  getPagePath() {
    return 'pages/Index';
  }
}
