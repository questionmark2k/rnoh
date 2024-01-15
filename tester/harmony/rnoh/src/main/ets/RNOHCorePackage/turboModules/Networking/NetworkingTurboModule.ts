import http from '@ohos.net.http'
import util from "@ohos.util";

import { TurboModule } from "../../../RNOH/TurboModule";
import { NetworkEventsDispatcher } from './NetworkEventDispatcher';
import ArrayList from '@ohos.util.ArrayList';

type ResponseType =
| 'base64'
  | 'blob'
  | 'text';

interface Query {
  method: string,
  url: string,
  data: Object,
  headers: Object,
  responseType: ResponseType,
  incrementalUpdates: boolean,
  timeout: number,
  withCredentials: boolean,
}

export type UriHandler = {
  supports: (query: Query) => boolean;
  fetch: (query: Query) => Object;
}

export class NetworkingTurboModule extends TurboModule {
  public static readonly NAME = 'Networking';
  private nextId: number = 0
  private requestMap: Map<number, http.HttpRequest> = new Map();
  private networkEventDispatcher: NetworkEventsDispatcher = new NetworkEventsDispatcher(this.ctx.rnInstance)
  private base64Helper: util.Base64Helper = new util.Base64Helper();
  private uriHandlers: ArrayList<UriHandler> = new ArrayList();

  private REQUEST_METHOD_BY_NAME: Record<string, http.RequestMethod> = {
    OPTIONS: http.RequestMethod.OPTIONS,
    GET: http.RequestMethod.GET,
    HEAD: http.RequestMethod.HEAD,
    POST: http.RequestMethod.POST,
    PUT: http.RequestMethod.PUT,
    DELETE: http.RequestMethod.DELETE,
    TRACE: http.RequestMethod.TRACE,
    CONNECT: http.RequestMethod.CONNECT,
  }

  addUriHandler(handler: UriHandler) {
    this.uriHandlers.add(handler);
  }

  removeUriHandler(handler: UriHandler) {
    this.uriHandlers.remove(handler);
  }

  decodeBuffer(buf: ArrayBuffer): string {
    const textDecoder = util.TextDecoder.create();
    const byteArray = new Uint8Array(buf);
    return textDecoder.decodeWithStream(byteArray, { stream: false });
  }

  async encodeResponse(response: string | Object | ArrayBuffer, responseType: ResponseType): Promise<string | Object> {
    if (responseType === 'text') {
      if (typeof response === 'string') {
        return response;
      } else if (response instanceof ArrayBuffer) {
        return this.decodeBuffer(response);
      } else {
        // NOTE: Object responses have been long deprecated in Ark, we don't expect them here
        throw new Error("INTERNAL: unexpected Object http response");
      }
    } else if (responseType === 'base64') {
      let byteArray: Uint8Array;
      if (typeof response === 'string') {
        const textEncoder = new util.TextEncoder();
        byteArray = textEncoder.encodeInto(response);
      } else if (response instanceof ArrayBuffer) {
        byteArray = new Uint8Array(response);
      } else {
        throw new Error("INTERNAL: unexpected Object http response");
      }
      return this.base64Helper.encodeToString(byteArray);
    }

    throw new Error("Unsupported query response type");
  }

  private encodeBody(data: Object): string | ArrayBuffer | Object {
    if ('string' in data) {
      return data.string as string
    }
    if ('base64' in data) {
      const base64 = data.base64 as string;
      const textEncoder = new util.TextEncoder();
      const byteArray = textEncoder.encodeInto(base64);
      return byteArray.buffer;
    }
    return data;
  }


  sendRequest(query: Query, callback: (requestId: number) => void) {
    const requestId = this.createId()
    const httpRequest = http.createHttp();

    for (const handler of this.uriHandlers) {
      if (handler.supports(query)) {
        const response = handler.fetch(query);
        this.networkEventDispatcher.dispatchDidReceiveNetworkData(requestId, response);
        this.networkEventDispatcher.dispatchDidCompleteNetworkResponse(requestId);
        return;
      }
    }

    const extraData = this.encodeBody(query.data);

    httpRequest.request(
      query.url,
      {
        method: this.REQUEST_METHOD_BY_NAME[query.method],
        header: query.headers,
        extraData: extraData,
        connectTimeout: query.timeout,
        readTimeout: query.timeout
      },
      async (err, data) => {
        if (!err) {
          this.networkEventDispatcher.dispatchDidReceiveNetworkResponse(requestId, data.responseCode, query.headers, query.url);
          this.networkEventDispatcher.dispatchDidReceiveNetworkData(requestId, await this.encodeResponse(data.result, query.responseType));
          this.networkEventDispatcher.dispatchDidCompleteNetworkResponse(requestId);
        } else {
          this.networkEventDispatcher.dispatchDidReceiveNetworkResponse(requestId, data?.responseCode ?? 0, {
          }, query.url)
          this.networkEventDispatcher.dispatchDidCompleteNetworkResponseWithError(requestId, err.toString());
        }
        httpRequest.destroy();
        this.requestMap.delete(requestId)
      }
    );

    this.requestMap.set(requestId, httpRequest);
    callback(requestId)
  }

  abortRequest(requestId: number) {
    const httpRequest = this.requestMap.get(requestId);
    if (httpRequest) {
      httpRequest.destroy()
      this.requestMap.delete(requestId)
    }
  }

  private createId(): number {
    return this.nextId++
  }
}