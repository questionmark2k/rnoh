import {
  AnyJSBundleProvider,
  MetroJSBundleProvider,
  ResourceJSBundleProvider,
  RNInstance,
  RNOHCoreContext,
  SurfaceHandle,
  Tag
} from 'rnoh/ts';
import { RNAbility } from 'rnoh';
import { createRNPackages } from '../RNPackagesFactory';


export default class EntryAbility extends RNAbility {
  protected async onPrepareRNInstance(rnohCoreContext: RNOHCoreContext): Promise<void> {
    super.onPrepareRNInstance(rnohCoreContext)
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

  protected getPagePath() {
    return 'pages/Index';
  }
}
