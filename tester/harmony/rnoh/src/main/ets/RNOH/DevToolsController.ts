import { RNInstanceRegistry } from "./RNInstanceRegistry"
import { EventEmitter } from "./EventEmitter"


export class DevToolsController {
  public eventEmitter = new EventEmitter<{ "RELOAD": [{ reason: string | undefined }] }>()

  constructor(private rnInstanceRegistry: RNInstanceRegistry) {
  }

  reload(reason: string | undefined): void {
    this.eventEmitter.emit("RELOAD", { reason })
  }

  toggleElementInspector(): void {
    this.rnInstanceRegistry.forEach(rnInstance => {
      rnInstance.emitDeviceEvent("toggleElementInspector", {});
    })
  }
}