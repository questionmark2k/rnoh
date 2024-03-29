import type { DescriptorRegistry } from './DescriptorRegistry';
import type { RNComponentCommandReceiver } from './RNComponentCommandHub';
import type { RNInstance, RNInstanceImpl } from './RNInstance';
import type { ComponentManagerRegistry } from './ComponentManagerRegistry';
import { RNOHCoreContext } from './RNOHCoreContext';

export class RNOHContext extends RNOHCoreContext {
  public get descriptorRegistry(): DescriptorRegistry {
    return this.rnInstance.descriptorRegistry;
  }

  public get componentCommandReceiver(): RNComponentCommandReceiver {
    return this.rnInstanceImpl.componentCommandHub;
  }

  public get componentManagerRegistry(): ComponentManagerRegistry {
    return this.rnInstance.componentManagerRegistry;
  }

  public get rnInstance(): RNInstance {
    return this.rnInstanceImpl
  }

  constructor(
    public reactNativeVersion: string,
    private rnInstanceImpl: RNInstanceImpl,
    coreContext: RNOHCoreContext
  ) {
    super(
      coreContext.logger,
      coreContext.getDisplayMetrics,
      coreContext.getUIAbilityState,
      coreContext.markReadiness,
      coreContext.uiAbilityContext,
      coreContext.devToolsController,
      coreContext.devMenu,
      coreContext.safeAreaInsetsProvider,
      coreContext.launchUri,
    )
    this.devToolsController = coreContext.devToolsController
    this.devMenu = coreContext.devMenu
    this.safeAreaInsetsProvider = coreContext.safeAreaInsetsProvider
    this.launchUri = coreContext.launchUri
    this.getDisplayMetrics = () => coreContext.getDisplayMetrics()
    this.getUIAbilityState = () => coreContext.getUIAbilityState()
    this.markReadiness = () => coreContext.markReadiness()
  }
}
