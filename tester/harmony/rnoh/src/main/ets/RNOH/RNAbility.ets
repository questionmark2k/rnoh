import UIAbility from '@ohos.app.ability.UIAbility';
import type { RNOHLogger } from './RNOHLogger';
import { StandardRNOHLogger } from './RNOHLogger';
import window from '@ohos.window';
import Want from '@ohos.app.ability.Want';
import { RNInstancesCoordinator } from "./RNInstancesCoordinator"
import AbilityConstant from '@ohos.app.ability.AbilityConstant';
import AbilityConfiguration from '@ohos.app.ability.Configuration';

/**
 Application developers are expected to extend this class. RNAbility aims to move much of the code to the RNOH side in order to improve stability.
 RNInstancesCoordinator can be used if more control is needed.
 */
export abstract class RNAbility extends UIAbility {
  private unregisterWindowListenerCallback = () => {
  }
  private rnInstancesCoordinator!: RNInstancesCoordinator
  private logger!: RNOHLogger

  onCreate(want: Want) {
    this.logger = this.createLogger().clone("RNAbility")
    this.rnInstancesCoordinator = RNInstancesCoordinator.create({
      logger: this.createLogger(),
      uiAbilityContext: this.context
    }, {
      launchURI: want.uri, onGetPackagerClientConfig: (buildMode) => buildMode === "DEBUG" ? {
        host: "localhost",
        port: 8081
      } : undefined
    })
    AppStorage.setOrCreate('RNOHCoreContext', this.rnInstancesCoordinator.getRNOHCoreContext())
  }

  onDestroy() {
    this.rnInstancesCoordinator?.onDestroy()
  }


  protected createLogger(): RNOHLogger {
    return new StandardRNOHLogger((err) => {
      this.rnInstancesCoordinator?.getRNOHCoreContext().reportRNOHError(err)
    });
  }

  onWindowStageCreate(windowStage: window.WindowStage) {
    const logger = this.logger.clone("onWindowStageCreate")
    const stopTracing = logger.startTracing()
    this.onWindowSetup(windowStage, logger).finally(stopTracing)
  }


  protected async onWindowSetup(windowStage: window.WindowStage, providedLogger: RNOHLogger) {
    const logger = providedLogger.clone("onWindowSetup");
    const stopTracing = logger.startTracing()

    try {
      const mainWindow = await windowStage.getMainWindow();

      const onWindowSizeChange = (windowSize: window.Size) => {
        this.rnInstancesCoordinator?.onWindowSizeChange(windowSize)
      }

      // subscribe to window size changes
      mainWindow.on('windowSizeChange', onWindowSizeChange)
      // unregister the previous callback, if present
      this.unregisterWindowListenerCallback()
      this.unregisterWindowListenerCallback = () => {
        try {
          mainWindow.off("windowSizeChange", onWindowSizeChange)
        } catch {
          this.logger?.error("Error when trying to unsubscribe from window size changes")
        }
      }

      await mainWindow.setWindowLayoutFullScreen(true)

      windowStage.loadContent(this.getPagePath(), (err, data) => {
        if (err.code) {
          logger.error("Failed to load the content", err.code)
          return;
        }
        logger.info("Succeeded in loading the content", JSON.stringify(data))
      });
      this.rnInstancesCoordinator.onWindowSizeChange(mainWindow.getWindowProperties().windowRect)
    } catch (err) {
      logger.error("Failed to setup window", JSON.stringify(err))
    } finally {
      stopTracing()
    }
  }


  onMemoryLevel(level: AbilityConstant.MemoryLevel) {
    this.rnInstancesCoordinator.onMemoryLevel(level)
  }

  onConfigurationUpdate(config: AbilityConfiguration.Configuration) {
    this.rnInstancesCoordinator.onConfigurationUpdate(config)
  }

  onForeground() {
    this.rnInstancesCoordinator.onForeground();
  }

  onBackground() {
    this.rnInstancesCoordinator.onBackground();
  }

  abstract getPagePath(): string
}
