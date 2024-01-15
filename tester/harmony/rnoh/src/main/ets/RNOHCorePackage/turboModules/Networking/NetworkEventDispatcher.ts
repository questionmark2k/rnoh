import { RNInstance } from '../../../RNOH/ts';

export class NetworkEventsDispatcher {
  constructor(private rnInstance: RNInstance) {
  }

  dispatchDidReceiveNetworkData(requestId: number, data: string | Object) {
    this.sendEvent("didReceiveNetworkData", [requestId, data])
  }

   dispatchDidReceiveNetworkResponse(requestId: number, status: number, headers: Object, url: string) {
    this.sendEvent("didReceiveNetworkResponse", [requestId, status, headers, url])
  }

   dispatchDidCompleteNetworkResponse(requestId: number) {
    this.sendEvent("didCompleteNetworkResponse", [requestId, ""])
  }

   dispatchDidCompleteNetworkResponseWithError(requestId: number, error: string) {
    this.sendEvent("didCompleteNetworkResponse", [requestId, error])
  }

  private  sendEvent(eventName: string, body: Object) {
    this.rnInstance.emitDeviceEvent(eventName, body)
  }
}
