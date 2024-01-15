import { TurboModule, TurboModuleContext } from '../../../RNOH/TurboModule';
import util from '@ohos.util';
import fs from '@ohos.file.fs';
import { ContentHandler, WebSocketTurboModule } from '../WebSocketTurboModule';
import { UriHandler } from '../Networking/index';
import uri from '@ohos.uri';
import { BlobMetaData, BlobPart } from './types';
import { BlobRegistry } from './BlobRegistry';


export class BlobTurboModule extends TurboModule {
  public static readonly NAME = 'BlobModule';
  private blobRegistry: BlobRegistry = new BlobRegistry()

  private contentHandler: ContentHandler = {
    processMessage: (params) => {
      return params;
    },
    processByteMessage: (bytes, params) => {
      return {
        id: params.id,
        type: "blob",
        data: {
          blobId: this.blobRegistry.saveArrayBuffer(bytes),
          offset: 0,
          size: bytes.byteLength
        }
      }
    }

  }

  private networkingUriHandler: UriHandler = {
    supports(query) {
      const parsedUri = new uri.URI(query.url)
      const remote = parsedUri.scheme === 'http' || parsedUri.scheme === 'https';
      if (!remote && query.responseType === 'blob') {
        return true;
      }
      return false;
    },
    async fetch(query) {
      const file = fs.openSync(query.url, fs.OpenMode.READ_WRITE);
      const stat = await fs.stat(query.url);
      const arrayBuffer = new ArrayBuffer(stat.size);
      await fs.read(file.fd, arrayBuffer);
      return {
        blobId: this.blobRegistry.store(arrayBuffer),
        offset: 0,
        size: stat.size,
        type: undefined, //no way to find the type using Ark
        name: file.name,
        lastModified: stat.mtime
      }
    }
  }


  private getWebSocketModule(): WebSocketTurboModule {
    return this.ctx.rnInstance.getTurboModule(WebSocketTurboModule.NAME);
  }


  /*
   * The below methods are called by NAPI
  */

  createFromParts(parts: Array<BlobPart>, blobId: string): void {
    this.blobRegistry.appendPartsToBlob(parts, blobId)
  }

  sendOverSocket(blob: BlobMetaData, idDouble: number): void {
    const id = Math.floor(idDouble);
    const webSocketModule = this.getWebSocketModule();

    if (webSocketModule) {
      const data = this.blobRegistry.findById(blob.blobId, blob.offset, blob.size);

      if (data) {
        webSocketModule.sendBinaryArray(new Uint8Array(data), id);
      } else {
        webSocketModule.sendBinary(null, id);
      }
    }
  }

  addWebSocketHandler(socketID: number) {
    const module = this.getWebSocketModule()
    module.setContentHandler(socketID, this.contentHandler);
  }

  removeWebSocketHandler(socketID: number) {
    const module = this.getWebSocketModule()
    module.setContentHandler(socketID, null);
  }

  release(blobId: string): void {
    this.blobRegistry.deleteBlobById(blobId)
  }

  getConstants() {
    return { BLOB_URI_SCHEME: 'blob', BLOB_URI_HOST: null };
  }
}
