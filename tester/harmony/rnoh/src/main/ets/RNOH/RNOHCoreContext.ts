import { DevMenu } from './DevMenu'
import { DevToolsController } from './DevToolsController'
import { RNOHLogger } from './RNOHLogger'
import { SafeAreaInsetsProvider } from './SafeAreaInsetsProvider'
import { DisplayMetrics } from './types'
import common from '@ohos.app.ability.common'

export type UIAbilityState = "FOREGROUND" | "BACKGROUND"

export class RNOHCoreContext {
  public static create(
    displayMetricsProvider: () => DisplayMetrics,
    uiAbilityStateProvider: () => UIAbilityState,
    onMarkReadiness: () => void,
    launchUri: string | undefined,
    logger: RNOHLogger,
    uiAbilityContext: common.UIAbilityContext,
  ): RNOHCoreContext {
    const devToolsController = new DevToolsController(logger);
    const devMenu = new DevMenu(devToolsController, uiAbilityContext, logger);
    const safeAreaInsetsProvider = new SafeAreaInsetsProvider(uiAbilityContext);
    return new RNOHCoreContext(
      logger,
      displayMetricsProvider,
      uiAbilityStateProvider,
      onMarkReadiness,
      devToolsController,
      devMenu,
      safeAreaInsetsProvider,
      launchUri,
    )
  }

  protected constructor(
    public logger: RNOHLogger,
    public getDisplayMetrics: () => DisplayMetrics,
    public getUIAbilityState: () => UIAbilityState,
    public markReadiness: () => void,
    public devToolsController: DevToolsController,
    public devMenu: DevMenu,
    public safeAreaInsetsProvider: SafeAreaInsetsProvider,
    public launchUri?: string,
  ) {
  }
}
