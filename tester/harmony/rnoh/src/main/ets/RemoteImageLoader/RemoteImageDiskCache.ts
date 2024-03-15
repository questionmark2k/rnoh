import {RemoteImageCache} from './RemoteImageCache';
import fs from '@ohos.file.fs';

const EMPTY_CACHE_KEY = '';

export class RemoteImageDiskCache extends RemoteImageCache<boolean> {
  private cacheDir: string;

  constructor(maxSize: number, cacheDir: string) {
    super(maxSize);
    this.cacheDir = cacheDir;
    const filenames: string[] = fs.listFileSync(cacheDir);

    // number of files in cache might be over maxSize but `listFile` cannot specify sort order by modification time
    filenames
      .map(filename => filename.replace('/', ''))
      .forEach(filename => {
        this.set(filename, true);
      });
  }

  remove(key: string): void {
    const cachedKey = this.getCacheKey(key);
    if (this.data.has(cachedKey)) {
      try {
        fs.unlinkSync(cachedKey);
      } catch (reason) {
        throw new Error('Cache file was not deleted ' + reason);
      }
      this.data.delete(cachedKey);
    }
  }

  clear(): void {
    this.data.forEach((_value, key) => {
      const cachedKey = this.getCacheKey(key);
      fs.unlink(cachedKey).catch(reason => {
        throw new Error('Cache file was not deleted ' + reason);
      });
    });
    this.data.clear();
  }

  set(key: string, value: boolean = true): void {
    const cachedKey = this.getCacheKey(key);
    if (cachedKey === EMPTY_CACHE_KEY) {
      console.warn('Cache key not provided, not using cache');
      return;
    }
    return super.set(cachedKey, value);
  }

  has(key: string): boolean {
    const cachedKey = this.getCacheKey(key);
    if (super.has(cachedKey)) {
      try {
        fs.statSync(this.getFilePath(cachedKey));
        return true;
      } catch {
        console.error('no cache');
      }
    }
    return false;
  }

  // this method does not check if the file exists and is also used to determine file path for prefetch
  public getLocation(key: string): string {
    const cachedKey = this.getCacheKey(key);
    return this.getFilePath(cachedKey);
  }

  private getFilePath(key: string): string {
    return this.cacheDir + '/' + key;
  }

  private getCacheKey(uri: string): string {
    if (uri === undefined) {
      // if multiple images would have the same uri, they would overwrite each other
      // but it's better than crashing the app
      console.warn('Cache key not provided, defaulting to empty cache key');
      return EMPTY_CACHE_KEY;
    }
    const reg = /[^a-zA-Z0-9 -]/g;
    return uri.replace(reg, '');
  }
}
