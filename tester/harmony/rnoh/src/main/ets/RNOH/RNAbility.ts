// @ts-ignore
import libRNOHApp from 'librnoh_app.so';
import UIAbility from '@ohos.app.ability.UIAbility';
import { NapiBridge } from './NapiBridge';
import type { RNOHLogger } from './RNOHLogger';
import { StandardRNOHLogger } from './RNOHLogger';
import window from '@ohos.window';
import type { TurboModuleProvider } from './TurboModuleProvider';
import { RNInstanceRegistry } from './RNInstanceRegistry';
import { RNInstance, RNInstanceImpl, RNInstanceOptions, FrameNodeFactory } from './RNInstance';
import { RNOHContext } from './RNOHContext';
import { JSPackagerClient, JSPackagerClientConfig } from './JSPackagerClient';
import { RNOHError } from './RNOHError';
import { DisplayMetrics } from './types';
import { DisplayMetricsManager } from './DisplayMetricsManager';
import Want from '@ohos.app.ability.Want';
import { RNOHCoreContext } from './RNOHCoreContext';

const RNOH_BANNER = '\n\n\n' +
  '██████╗ ███╗   ██╗ ██████╗ ██╗  ██╗' + '\n' +
  '██╔══██╗████╗  ██║██╔═══██╗██║  ██║' + '\n' +
  '██████╔╝██╔██╗ ██║██║   ██║███████║' + '\n' +
  '██╔══██╗██║╚██╗██║██║   ██║██╔══██║' + '\n' +
  '██║  ██║██║ ╚████║╚██████╔╝██║  ██║' + '\n' +
  '╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝' + '\n\n'


export abstract class RNAbility extends UIAbility {
  public launchUri?: string
  protected storage: LocalStorage
  protected napiBridge: NapiBridge = null
  protected turboModuleProvider: TurboModuleProvider
  protected providedLogger: RNOHLogger
  protected logger: RNOHLogger
  protected rnInstanceRegistry: RNInstanceRegistry
  protected window: window.Window | undefined
  protected initializationDateTime: Date
  protected readinessDateTime: Date | undefined
  protected isDebugModeEnabled_: boolean = true
  protected jsPackagerClient: JSPackagerClient | undefined = undefined
  protected displayMetricsManager: DisplayMetricsManager = undefined
  private unregisterWindowListenerCallback: (() => void) | undefined = undefined;

  public rnohCoreContext: RNOHCoreContext;
  private inForeground: boolean = false;

  onCreate(want: Want, _param) {
    this.initializationDateTime = new Date()
    this.providedLogger = this.createLogger()
    this.providedLogger.info(RNOH_BANNER)
    this.logger = this.providedLogger.clone("RNAbility")
    const stopTracing = this.logger.clone("onCreate").startTracing()
    this.launchUri = want.uri
    this.napiBridge = new NapiBridge(libRNOHApp, this.providedLogger)
    const { isDebugModeEnabled } = this.napiBridge.onInit(this.shouldCleanUpRNInstance__hack())
    this.isDebugModeEnabled_ = isDebugModeEnabled
    if (this.logger instanceof StandardRNOHLogger) {
      this.logger.setMinSeverity(this.isDebugModeEnabled_ ? "debug" : "info")
    }
    if (this.isDebugModeEnabled_) {
      this.logger.warn("Debug mode is enabled. Performance is affected.")
    }
    this.rnInstanceRegistry = new RNInstanceRegistry(
      this.providedLogger,
      this.napiBridge,
      this.context,
      (rnInstance) => this.createRNOHContext({
        rnInstance
      }))
    this.rnohCoreContext = RNOHCoreContext.create(
      this.getDisplayMetrics.bind(this),
      this.getUIAbilityState.bind(this),
      this.markReadiness.bind(this),
      this.launchUri,
      this.providedLogger,
      this.context
    );
    this.jsPackagerClient = new JSPackagerClient(
      this.providedLogger,
      this.rnohCoreContext.devToolsController,
      this.rnohCoreContext.devMenu
    )
    const jsPackagerClientConfig = this.getJSPackagerClientConfig()
    if (jsPackagerClientConfig) {
      this.jsPackagerClient.connectToMetroMessages(jsPackagerClientConfig)
    }
    this.displayMetricsManager = new DisplayMetricsManager(this.logger);
    AppStorage.setOrCreate('RNAbility', this)
    stopTracing()
  }

  onDestroy() {
    const stopTracing = this.logger.clone("onDestroy").startTracing()
    this.jsPackagerClient.onDestroy()
    this.rnInstanceRegistry.forEach(instance => instance.onDestroy())
    this.unregisterWindowListenerCallback?.();
    stopTracing()
  }

  public isDebugModeEnabled() {
    return this.isDebugModeEnabled_
  }

  public reportError(err: RNOHError) {
    this.rnohCoreContext.devToolsController.setLastError(err)
    this.rnohCoreContext.devToolsController.eventEmitter.emit("NEW_ERROR", err)
  }

  protected getJSPackagerClientConfig(): JSPackagerClientConfig | null {
    if (!this.isDebugModeEnabled_) {
      return null
    }
    return {
      host: "localhost",
      port: 8081
    }
  }

  protected shouldCleanUpRNInstance__hack(): boolean {
    return false
  }

  public markReadiness(): void {
    if (!this.readinessDateTime) {
      this.readinessDateTime = new Date()
      this.logger.warn(`START UP TIME: ${this.readinessDateTime.getTime() - this.initializationDateTime.getTime()} ms`)
    }
  }

  public async createAndRegisterRNInstance(options: RNInstanceOptions, frameNodeFactory?: FrameNodeFactory): Promise<RNInstance> {
    const stopTracing = this.logger.clone("createAndRegisterRNInstance").startTracing()
    const result = await this.rnInstanceRegistry.createInstance({
      enableDebugger: this.isDebugModeEnabled_,
      devToolsController: this.rnohCoreContext.devToolsController,
      frameNodeFactory,
      ...options,

    })
    stopTracing()
    return result
  }

  public destroyAndUnregisterRNInstance(rnInstance: RNInstance) {
    const stopTracing = this.logger.clone("destroyAndUnregisterRNInstance").startTracing()
    if (rnInstance instanceof RNInstanceImpl) {
      rnInstance.onDestroy()
    }
    this.rnInstanceRegistry.deleteInstance(rnInstance.getId())
    stopTracing()
  }

  public createRNOHContext({rnInstance}: { rnInstance: RNInstance }) {
    if (!(rnInstance instanceof RNInstanceImpl)) {
      throw new Error("RNInstance must extend RNInstanceImpl")
    }
    return new RNOHContext(
      "0.72.5",
      rnInstance,
      this.rnohCoreContext)
  }

  protected createLogger(): RNOHLogger {
    return new StandardRNOHLogger(this.reportError.bind(this));
  }

  public getLogger(): RNOHLogger {
    return this.providedLogger
  }

  public async onWindowSetup(windowStage: window.WindowStage, providedLogger: RNOHLogger) {
    const logger = providedLogger.clone("onWindowSetup");
    const stopTracing = logger.startTracing()

    try {
      const mainWindow = await windowStage.getMainWindow();

      const onWindowSizeChange = (windowSize) => {
        this.displayMetricsManager.updateWindowSize(windowSize);
        this.rnInstanceRegistry?.forEach((rnInstance) => rnInstance.onWindowSizeChange(windowSize))
      }

      // subscribe to window size changes
      mainWindow.on('windowSizeChange', onWindowSizeChange)
      // unregister the previous callback, if present
      this.unregisterWindowListenerCallback?.()
      this.unregisterWindowListenerCallback = () => {
        try {
          mainWindow.off("windowSizeChange", onWindowSizeChange)
        } catch {
          this.logger.error("Error when trying to unsubscribe from window size changes")
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

      this.displayMetricsManager.updateDisplayMetrics(mainWindow.getWindowProperties().windowRect)
    } catch (err) {
      logger.error("Failed to setup window", JSON.stringify(err))
    } finally {
      stopTracing()
    }
  }

  onWindowStageCreate(windowStage: window.WindowStage) {
    const logger = this.logger.clone("onWindowStageCreate")
    const stopTracing = logger.startTracing()
    this.onWindowSetup(windowStage, logger).finally(stopTracing)
  }

  onMemoryLevel(level) {
    const stopTracing = this.logger.clone("onMemoryLevel").startTracing()
    const MEMORY_LEVEL_NAMES = ["MEMORY_LEVEL_MODERATE", "MEMORY_LEVEL_LOW", "MEMORY_LEVEL_CRITICAL"]
    this.logger?.debug("Received memory level event: " + MEMORY_LEVEL_NAMES[level])
    this.napiBridge.onMemoryLevel(level)
    stopTracing()
  }

  onConfigurationUpdate(config) {
    const stopTracing = this.logger?.clone("onConfigurationUpdate").startTracing()
    this.displayMetricsManager.updateDisplayMetrics()
    this.rnInstanceRegistry?.forEach((rnInstance) => rnInstance.onConfigurationUpdate(config))
    stopTracing()
  }

  onForeground() {
    const stopTracing = this.logger.clone("onForeground").startTracing()
    this.inForeground = true
    this.rnInstanceRegistry?.forEach((rnInstance) => rnInstance.onForeground())
    stopTracing()
  }

  onBackground() {
    const stopTracing = this.logger.clone("onBackground").startTracing()
    this.inForeground = false
    this.rnInstanceRegistry?.forEach((rnInstance) => rnInstance.onBackground())
    stopTracing()
  }

  public getUIAbilityState(): "FOREGROUND" | "BACKGROUND" {
    return this.inForeground ? "FOREGROUND" : "BACKGROUND";
  }

  onBackPress() {
    const stopTracing = this.logger.clone("onBackPress").startTracing()
    this.rnInstanceRegistry?.forEach((rnInstance) => rnInstance.onBackPress())
    stopTracing()
    return true;
  }

  abstract getPagePath(): string

  public getDisplayMetrics(): DisplayMetrics {
    return this.displayMetricsManager.getDisplayMetrics();
  }
}
