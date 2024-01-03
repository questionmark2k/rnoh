import image from '@ohos.multimedia.image'
import type { RemoteImageMemoryCache } from "./RemoteImageCache"
import { RemoteImageLoaderError } from "./RemoteImageLoaderError"
import request from '@ohos.request'
import type common from '@ohos.app.ability.common'
import type { RemoteImageDiskCache } from './RemoteImageDiskCache'
import fs from '@ohos.file.fs';
import { fetchDataFromUrl, FetchResult } from '../RNOH/HttpRequestHelper'
import { RemoteImageSource } from './RemoteImageSource'

export class RemoteImageLoader {
  private activeRequestByUrl: Map<string, Promise<FetchResult>> = new Map();
  private activePrefetchByUrl: Map<string, Promise<boolean>> = new Map();

  public constructor(
    private memoryCache: RemoteImageMemoryCache,
    private diskCache: RemoteImageDiskCache,
    private context: common.UIAbilityContext) {
  }

  private async fetchImage(url: string): Promise<FetchResult> {
    if (this.activeRequestByUrl.has(url)) {
      return this.activeRequestByUrl.get(url);
    }

    const promise = fetchDataFromUrl(url);
    this.activeRequestByUrl.set(url, promise);
    promise.then(() => {
      this.activeRequestByUrl.delete(url);
    }).catch(() => {
      this.activeRequestByUrl.delete(url);
    });
    return promise;
  }

  public async getPixelMap(uri: string): Promise<image.PixelMap> {
    const imageSource = await this.getImageSource(uri);
    return await imageSource.getPixelMapPromise();
  }

  public async getImageSource(uri: string): Promise<RemoteImageSource> {
    if (this.memoryCache.has(uri)) {
      return this.memoryCache.get(uri)
    }

    // if the image is being prefetched, wait until it's downloaded,
    // ignoring any errors that occur (we'll try to download it to memory instead)
    if (this.activePrefetchByUrl.has(uri)) {
      await this.activePrefetchByUrl.get(uri).catch(() => {
      });
    }

    if (this.diskCache.has(uri)) {
      const location = this.diskCache.get(uri);
      const imageSource = image.createImageSource(location);
      if (imageSource === null) {
        throw new RemoteImageLoaderError("Couldn't create ImageSource")
      }
      const remoteImageSource = new RemoteImageSource(imageSource, location);
      this.memoryCache.set(uri, remoteImageSource);
      return remoteImageSource;
    }

    let response: FetchResult
    try {
      response = await this.fetchImage(uri);
    } catch (e) {
      throw new RemoteImageLoaderError(e.message ?? "Failed to fetch the image");
    }

    // if more than one component tried fetching at the same time,
    // it's possible the source is now in the cache.
    // Check it first before creating a new source
    if (this.memoryCache.has(uri)) {
      return this.memoryCache.get(uri);
    }

    const imageSource = image.createImageSource(response.result)
    if (imageSource === null) {
      throw new RemoteImageLoaderError("Couldn't create ImageSource")
    }
    // NOTE: workaround for `Image` component not supporting redirects or ImageSource sources.
    // If the image is an animated GIF, we cannot convert it to a PixelMap
    // since that would display a single frame only.
    // Instead, when redirected, we attach the `location` url to the imageSource object
    // and use it as the source parameter for `Image`
    const location = response.headers['location'] ?? uri;
    const remoteImageSource = new RemoteImageSource(imageSource, location);
    this.memoryCache.set(uri, remoteImageSource)
    return remoteImageSource;
  }

  public async prefetch(uri: string): Promise<boolean> {
    if (this.diskCache.has(uri)) {
      return true;
    }

    if (this.activePrefetchByUrl.has(uri)) {
      return await this.activePrefetchByUrl.get(uri);
    }

    // evict the image from cache if present
    if (this.memoryCache.has(uri)) {
      this.memoryCache.remove(uri);
    }

    const reg = /[^a-zA-Z0-9 -]/g;
    const path = `${this.context.cacheDir}/${uri.replace(reg, '')}`;

    const promise = (async () => {
      try {
        await request.downloadFile(this.context, { url: uri, filePath: path });
        this.diskCache.set(uri, `file://${path}`);
      } catch (e) {
        // request.downloadFile does not allow overwriting,
        // so we create a temp file and override the old one manually
        if (e.code === request.EXCEPTION_FILEPATH) {
          try {
            const tempPath = path + "_temp";
            await request.downloadFile(this.context, { url: uri, filePath: tempPath });
            await fs.moveFile(tempPath, path);
            this.diskCache.set(uri, `file://${path}`)
          } catch (e1) {
            return Promise.reject("Failed to fetch the image")
          }
        } else {
          return Promise.reject("Failed to fetch the image")
        }
      }

      return true
    })()
    this.activePrefetchByUrl.set(uri, promise);
    promise.then(() => {
      this.activePrefetchByUrl.delete(uri)
    }).catch(() => {
      this.activePrefetchByUrl.delete(uri)
    });

    return await promise;
  }

  public getImageFromCache(uri: string): string | undefined {
    if (this.diskCache.has(uri)) {
      return this.diskCache.get(uri)
    }

    return undefined;
  }

  public queryCache(uri: string): 'memory' | 'disk' | undefined {
    if (this.diskCache.has(uri)) {
      return 'disk';
    }
    if (this.memoryCache.has(uri)) {
      return 'memory';
    }
    return undefined;
  }
}