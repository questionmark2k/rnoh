import url from '@ohos.url'
import http from '@ohos.net.http';
import { InspectorPackagerConnection } from './InspectorPackagerConnection';
import { RNOHLogger } from './RNOHLogger';
import { InspectorInstance } from './types';

function getServerHost(bundleUrlString: string): string {
  const bundleUrl = url.URL.parseURL(bundleUrlString);
  const host = bundleUrl.hostname ?? "localhost";
  const port = bundleUrl.port ?? "8081";
  return `${host}:${port}`;
}

// returns the inspector url for current device and application
function getInspectorDeviceUrl(bundleUrl: string) {
  // TODO:
  const deviceName = "HarmonyDevice";
  const appName = "com.rnoh.tester";
  return `http://${getServerHost(bundleUrl)}/inspector/device?name=${deviceName}&app=${appName}`;
}

// This is a port of the Android impl, at
// ReactAndroid/src/main/java/com/facebook/react/devsupport/DevServerHelper.java
export class DevServerHelper {
  static DEBUGGER_MSG_DISABLE = "{ \"id\":1,\"method\":\"Debugger.disable\" }";
  static connectionByInspectorUrl = new Map<string, InspectorPackagerConnection>();

  public static connectToDevServer(bundleUrl: string, logger: RNOHLogger, inspectorInstance: InspectorInstance) {
    const inspectorUrl = getInspectorDeviceUrl(bundleUrl);
    let connection = DevServerHelper.connectionByInspectorUrl.get(inspectorUrl);

    if (!connection || !connection.isConnected()) {
      connection = new InspectorPackagerConnection(inspectorUrl, logger, inspectorInstance);
      DevServerHelper.connectionByInspectorUrl.set(inspectorUrl, connection);
      connection.connect();
    }

    return connection;
  }

  public static openUrl(url: string, bundleUrl: string, onErrorCallback: () => void) {
    const openUrlEndpoint = `http://${getServerHost(bundleUrl)}/open-url`;
    const body = JSON.stringify({ url });

    const httpRequest = http.createHttp();
    httpRequest.request(
      openUrlEndpoint,
      {
        extraData: body,
        method: http.RequestMethod.POST
      },
      (err) => {
        if (err) {
          onErrorCallback();
        }
      });
  }

  public static disableDebugger()
  {
    DevServerHelper.connectionByInspectorUrl.forEach(connection => {
      connection.sendEventToAllConnections(DevServerHelper.DEBUGGER_MSG_DISABLE);
    })
  }
}
