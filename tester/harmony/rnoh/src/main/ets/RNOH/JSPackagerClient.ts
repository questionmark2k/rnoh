import webSocket from '@ohos.net.webSocket';
import { DevMenuTurboModule } from '../RNOHCorePackage/turboModules';
import { RNOHLogger } from './RNOHLogger';
import { RNInstance } from './ts';

export interface JSPackagerClientConfig {
  host: string,
  port: number | string,
}

export class JSPackagerClient {
  private webSocket: webSocket.WebSocket;
  private logger: RNOHLogger;
  private rnInstance: RNInstance;

  constructor(logger: RNOHLogger, rnInstance: RNInstance) {
    this.logger = logger.clone("JSPackagerClient");
    this.rnInstance = rnInstance;
  }

  public connectToMetroMessages(config: JSPackagerClientConfig) {
    this.webSocket = webSocket.createWebSocket();
    const url = this.buildUrl(config);
    this.webSocket.on("message", (err, data) => {
      if (err) {
        this.logger.error("Websocket error " + err.message);
        return;
      }
      if (typeof data === "string") {
        const message = JSON.parse(data);
        const devMenuTurboModule = this.rnInstance.getTurboModule<DevMenuTurboModule>(DevMenuTurboModule.NAME);
        switch (message.method) {
          case "devMenu":
            devMenuTurboModule.show();
            break;
          case "reload":
            devMenuTurboModule.reload();
            break;
          default:
            this.logger.warn(`Unsupported action: ${message.method}`)
        }
      }
    })
    this.webSocket.connect(url, (err, _data) => {
      if (!err) {
        this.logger.info("Websocket connected successfully");
      } else {
        this.logger.error("Websocket connection failed, err: " + JSON.stringify(err));
      }
    });
  }

  public onDestroy() {
    this.webSocket?.close();
  }

  private buildUrl(config: JSPackagerClientConfig): string {
    return `ws://${config.host}:${config.port}/message`;
  }
}