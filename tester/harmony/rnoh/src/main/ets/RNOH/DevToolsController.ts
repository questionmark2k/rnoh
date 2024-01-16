import { RNInstanceRegistry } from "./RNInstanceRegistry"
import { EventEmitter } from "./EventEmitter"
import { DevServerHelper } from "./DevServerHelper"
import { RNOHLogger } from './RNOHLogger'

export class DevToolsController {
  public eventEmitter = new EventEmitter<{
    "RELOAD": [{ reason: string | undefined }],
    "DISMISS_RNOH_ERROR_DIALOG",
    "SHOW_DEV_LOADING_VIEW": [
      rnInstanceId: number,
      message: string,
      color?: number,
      backgroundColor?: number
    ],
    "HIDE_DEV_LOADING_VIEW": [rnInstanceId: number,
    ]
  }>()

  private logger: RNOHLogger;

  constructor(private rnInstanceRegistry: RNInstanceRegistry, logger: RNOHLogger) {
    this.logger = logger.clone("DevMenu");
  }

  reload(reason: string | undefined = undefined): void {
    // Disable debugger to resume the JsVM & avoid thread locks while reloading
    DevServerHelper.disableDebugger();
    this.eventEmitter.emit("RELOAD", { reason })
  }

  toggleElementInspector(): void {
    this.rnInstanceRegistry.forEach(rnInstance => {
      rnInstance.emitDeviceEvent("toggleElementInspector", {});
    })
  }

  emitDevMenuShown(): void {
    this.rnInstanceRegistry.forEach(rnInstance => {
      rnInstance.emitDeviceEvent("RCTDevMenuShown", {});
    })
  }

  dismissRNOHErrorDialog(): void {
    this.eventEmitter.emit("DISMISS_RNOH_ERROR_DIALOG")
  }

  openDebugger(): void {
    this.rnInstanceRegistry.forEach(instance => {
      DevServerHelper.openUrl(
        "flipper://null/Hermesdebuggerrn?device=React%20Native",
        instance.getInitialBundleUrl(),
        () => this.logger.error("Failed to open Flipper. Please check that Metro is running.")
      );
    })
  }

  openDevTools(): void {
    this.rnInstanceRegistry.forEach(instance => {
      DevServerHelper.openUrl(
        "flipper://null/React?device=React%20Native",
        instance.getInitialBundleUrl(),
        () => this.logger.error("Failed to open Flipper. Please check that Metro is running.")
      );
    })
  }
}