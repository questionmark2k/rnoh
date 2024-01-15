import Url from '@ohos.url'
import util from '@ohos.util';
import { BlobMetaData, BlobPart } from './types';

export class BlobRegistry {
  private blobById: Map<string, ArrayBuffer> = new Map();

  findByUri(uri: string): ArrayBuffer | null {
    const url = Url.URL.parseURL(uri);
    const urlParams = new Url.URLParams(url.search)
    const blobId: string | undefined = url.pathname.split("/").pop();
    let offset = 0;
    let size = -1;
    const offsetParam = urlParams.get("offset");
    if (offsetParam !== null) {
      offset = parseInt(offsetParam, 10);
    }
    const sizeParam = urlParams.get("size");
    if (sizeParam !== null) {
      size = parseInt(sizeParam, 10);
    }
    return this.findById(blobId ?? '', offset, size);
  }

  findById(blobId: string, offset: number, size: number): ArrayBuffer | null {
    const data = this.blobById.get(blobId);
    if (!data) {
      return null;
    }
    if (size === -1) {
      size = data.byteLength - offset;
    }

    if (offset > 0 || size !== data.byteLength) {
      const bytes = new Uint8Array(data, offset, size);
      return bytes.buffer;
    }
    return data;
  }

  findByMetaData(blob: BlobMetaData): ArrayBuffer | null {
    return this.findById(blob.blobId, blob.offset, blob.size);
  }

  appendPartsToBlob(parts: Array<BlobPart>, blobId: string): void {
    let totalBlobSize: number = 0;
    let partList: Array<ArrayBuffer> = new Array<ArrayBuffer>(parts.length);

    for (let i = 0; i < parts.length; i++) {
      let part = parts[i];
      switch (part.type) {
        case "blob":
          let blob = part.data as BlobMetaData;
          totalBlobSize += blob.size;
          partList[i] = this.findByMetaData(blob);
          break;
        case "string":
          let textEncoder = new util.TextEncoder();
          let bytes: ArrayBuffer = textEncoder.encodeInto(part.data as string).buffer;
          totalBlobSize += bytes.byteLength;
          partList[i] = bytes;
          break;
        default:
          throw new Error("Invalid type for blob: " + part["type"]);
      }
    }
    let buffer: ArrayBuffer = new ArrayBuffer(totalBlobSize);
    let bufferView = new Uint8Array(buffer);
    let pos = 0;
    for (let part of partList) {
      let partView = new Uint8Array(part);
      bufferView.set(partView, pos);
      pos += part.byteLength;
    }
    this.saveArrayBufferWithId(buffer, blobId);
  }

  saveArrayBuffer(data: ArrayBuffer): string {
    const blobId: string = util.generateRandomUUID();
    this.saveArrayBufferWithId(data, blobId);
    return blobId;
  }

  saveArrayBufferWithId(data: ArrayBuffer, blobId: string): void {
    this.blobById.set(blobId, data);
  }

  deleteBlobById(blobId: string) {
    this.blobById.delete(blobId);
  }
}