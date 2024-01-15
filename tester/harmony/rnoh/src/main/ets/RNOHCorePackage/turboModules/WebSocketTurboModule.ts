import webSocket from '@ohos.net.webSocket'
import util from '@ohos.util'
import { TurboModule, TurboModuleContext, RNOHLogger } from "../../RNOH/ts";
import { BusinessError } from '@ohos.base';
import { BlobMetaData } from './Blob';

const WEB_SOCKET_SUPPORTED_EVENT_NAMES = ["websocketOpen", "websocketClosed", "websocketFailed", "websocketMessage"] as const;

type MessageParams = {
  id: number,
  type: string,
  data?: string | BlobMetaData,
}

export type ContentHandler = {
  processMessage: (params: MessageParams) => MessageParams;
  processByteMessage: (byteString: ArrayBuffer, params: MessageParams) => MessageParams;
}

export class WebSocketTurboModule extends TurboModule {
  public static readonly NAME = 'WebSocketModule';

  private socketsById: Map<number, webSocket.WebSocket> = new Map();
  private logger: RNOHLogger
  private base64 = new util.Base64Helper();
  private contentHandlersBySocketID: Map<number, ContentHandler> = new Map();

  constructor(ctx: TurboModuleContext) {
    super(ctx)
    this.logger = ctx.logger.clone("WebSocketTurboModule")
  }


  getSupportedEvents() {
    return WEB_SOCKET_SUPPORTED_EVENT_NAMES
  }

  setContentHandler(socketID: number, contentHandler?: ContentHandler) {
    if (contentHandler) {
      this.contentHandlersBySocketID.set(socketID, contentHandler);
    } else {
      this.contentHandlersBySocketID.delete(socketID);
    }
  }

  private onMessage(err: BusinessError<void>, data: string | ArrayBuffer, socketID:number) {
    const contentHandler = this.contentHandlersBySocketID.get(socketID);

    if (typeof data === "string") {
      let params: MessageParams = { id: socketID, type: 'text', data: data };
      if (contentHandler) {
        params = contentHandler.processMessage(params);
      }

      this.ctx.rnInstanceManager.emitDeviceEvent("websocketMessage", params);
    } else if (data instanceof ArrayBuffer) {
      const base64Data = this.base64.encodeToStringSync(new Uint8Array(data));
      let params: MessageParams = { id: socketID, type: 'text', data: base64Data };
      if (contentHandler) {
        params = contentHandler.processByteMessage(data, params);
      }

      this.ctx.rnInstanceManager.emitDeviceEvent("websocketMessage", params);
    }
  }

  connect(url: string, protocols: string[] | undefined | null, options: { headers?: object }, socketID: number) {
    this.logger.clone("connect").debug(url)
    const ws = webSocket.createWebSocket();

    ws.on('open', (data) => {
      this.ctx.rnInstanceManager.emitDeviceEvent("websocketOpen", {
        id: socketID,
        protocol: "",
      });
    });

    ws.on('error', (err) => this.handleError(socketID, err));
    ws.on('message', (err, data) => this.onMessage(err, data, socketID));
    ws.on('close', (err, data) => {
      this.ctx.rnInstanceManager.emitDeviceEvent("websocketClosed", {
        id: socketID,
        ...data,
      })

    })

    ws.connect(url, { header: options.headers }, (err) => this.handleError(socketID, err));
    this.socketsById.set(socketID, ws);
  }

  send(message: string, socketID: number) {
    const ws = this.socketsById.get(socketID);
    if (!ws) {
      throw new Error(`Trying to send a message on websocket "${socketID}" but there is no socket.`);
    }

    ws.send(message, (err) => this.handleError(socketID, err));
  }

  sendBinary(base64Message: string, socketID: number) {
    const ws = this.socketsById.get(socketID);
    if (!ws) {
      throw new Error(`Trying to send a message on websocket "${socketID}" but there is no socket.`);
    }

    const message = this.base64.decodeSync(base64Message);
    ws.send(message.buffer, (err) => this.handleError(socketID, err));
  }

  sendBinaryArray(message: Uint8Array, socketID: number) {
    const ws = this.socketsById.get(socketID);
    if (!ws) {
      throw new Error(`Trying to send a message on websocket "${socketID}" but there is no socket.`);
    }

    ws.send(message.buffer, (err) => this.handleError(socketID, err));
  }

  ping(socketID: number) {
    const ws = this.socketsById.get(socketID);
    if (!ws) {
      throw new Error(`Trying to send a ping on websocket "${socketID}" but there is no socket.`);
    }

    this.ctx.logger.warn("WebSocketTurboModule::ping not implemented");
  }

  close(code: number, reason: string, socketID: number) {
    const ws = this.socketsById.get(socketID);
    if (!ws) {
      throw new Error(`Trying to close websocket "${socketID}" but there is no socket.`);
    }

    ws.close({ code, reason }, (err) => this.handleError(socketID, err));
    this.socketsById.delete(socketID);
    this.contentHandlersBySocketID.delete(socketID);
  }

  private handleError(socketID: number, err) {
    if (err) {
      this.ctx.logger.info(`WebSocketTurboModule::handleError ${JSON.stringify(err)}`);
      this.ctx.rnInstanceManager.emitDeviceEvent("websocketFailed", { id: socketID, message: JSON.stringify(err) });
      this.socketsById.delete(socketID);
      this.contentHandlersBySocketID.delete(socketID);
    }
  }
}
