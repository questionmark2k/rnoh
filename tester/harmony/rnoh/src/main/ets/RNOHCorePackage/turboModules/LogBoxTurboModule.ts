import { TurboModule, EventEmitter } from "../../RNOH/ts"


export class LogBoxTurboModule extends TurboModule {
  static NAME: Readonly<string> = "LogBox"

  public eventEmitter = new EventEmitter<{
    "SHOW": [],
    "HIDE": []
  }>()

  public show() {
    this.eventEmitter.emit("SHOW")
  }

  public hide() {
    this.eventEmitter.emit("HIDE")
  }
}