import type common from '@ohos.app.ability.common';
import type { DescriptorRegistry } from './DescriptorRegistry';
import type { RNComponentCommandHub, RNComponentCommandReceiver } from './RNComponentCommandHub';
import type { RNOHLogger } from './RNOHLogger';
import type { RNInstance, RNInstanceImpl } from './RNInstance';
import type { ComponentManagerRegistry } from './ComponentManagerRegistry';
import { RNOHCoreContext } from './RNOHCoreContext';

export class RNOHContext extends RNOHCoreContext {
  public get descriptorRegistry(): DescriptorRegistry {
    return this.rnInstance.descriptorRegistry;
  }

  /**
   * @deprecated: Use componentCommandReceiver instead.
   */
  public get commandDispatcher(): RNComponentCommandHub {
    return this.rnInstanceImpl.componentCommandHub;
  }

  public get componentCommandReceiver(): RNComponentCommandReceiver {
    return this.rnInstanceImpl.componentCommandHub;
  }

  public get uiAbilityContext(): common.UIAbilityContext {
    return this.rnInstance.abilityContext;
  }

  public get componentManagerRegistry(): ComponentManagerRegistry {
    return this.rnInstance.componentManagerRegistry;
  }

  /**
   * @deprecated Use `rnInstance` instead.
   */
  public get rnInstanceManager(): RNInstance {
    return this.rnInstance;
  }

  public get rnInstance(): RNInstance {
    return this.rnInstanceImpl
  }

  constructor(
    public reactNativeVersion: string,
    private rnInstanceImpl: RNInstanceImpl,
    public coreContext: RNOHCoreContext
  ) {
    super(
      coreContext.logger,
      coreContext.getDisplayMetrics,
      coreContext.getUIAbilityState,
      coreContext.markReadiness,
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
