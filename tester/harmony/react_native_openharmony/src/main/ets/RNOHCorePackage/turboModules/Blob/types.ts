export type Blob = ArrayBuffer;

export type BlobMetadata = {
  blobId: string,
  offset: number,
  size: number,
  name?: string,
  type?: string,
  lastModified?: number,
};

export type BlobPart = {
  type: string,
  data: string | BlobMetadata,
}