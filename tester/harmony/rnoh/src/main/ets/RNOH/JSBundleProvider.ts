import type resmgr from '@ohos.resourceManager';
import util from '@ohos.util';
import { RNOHError } from "./RNOHError"
import { RNOHLogger } from './RNOHLogger';
import urlUtils from '@ohos.url';
import { fetchDataFromUrl } from './HttpRequestHelper';
import fs from '@ohos.file.fs';

export interface HotReloadConfig {
  bundleEntry: string,
  host: string,
  port: number | string,
  scheme?: string,
}

export abstract class JSBundleProvider {
  abstract getURL(): string

  abstract getBundle(onProgress?: (progress: number) => void): Promise<ArrayBuffer>

  abstract getAppKeys(): string[]

  getHotReloadConfig(): HotReloadConfig | null {
    return null
  }

  getHumanFriendlyURL(): string {
    return this.getURL()
  }
}


export class JSBundleProviderError extends RNOHError {
}

export class MetroJSBundleProviderError extends JSBundleProviderError {}

export class FileJSBundleProvider extends JSBundleProvider {
  constructor(private path: string, private appKeys: string[] = []) {
    super()
  }

  getURL(): string {
    return this.path
  }

  getAppKeys(): string[] {
    return this.appKeys
  }

  async getBundle(onProgress?: (progress: number) => void): Promise<ArrayBuffer> {
    try {
      const file = await fs.open(this.path, fs.OpenMode.READ_ONLY);
      const { size } = await fs.stat(file.fd);
      const buffer = new ArrayBuffer(size);
      await fs.read(file.fd, buffer, { length: size });
      return buffer;
    } catch (err) {
      throw new JSBundleProviderError({
        whatHappened: `Couldn't load JSBundle from ${this.path}`,
        extraData: err,
        howCanItBeFixed: [`Check if a bundle exists at "${this.path}" on your device.`]
      })
    }
  }
}

export class ResourceJSBundleProvider extends JSBundleProvider {
  constructor(private resourceManager: resmgr.ResourceManager, private path: string = "bundle.harmony.js", private appKeys: string[] = []) {
    super()
  }

  getURL() {
    return this.path
  }

  getAppKeys() {
    return this.appKeys
  }

  async getBundle(onProgress?: (progress: number) => void) {
    try {
      const bundleFileContent = await this.resourceManager.getRawFileContent(this.path);
      const bundle = bundleFileContent.buffer;
      return bundle;
    } catch (err) {
      throw new JSBundleProviderError({
        whatHappened: `Couldn't load JSBundle from ${this.path}`,
        extraData: err,
        howCanItBeFixed: [`Check if a bundle exists at "<YOUR_ENTRY_MODULE>/src/main/resources/rawfile/${this.path}". (You can create a JS bundle with "react-native bundle-harmony" command.`]
      })
    }
  }
}


export class MetroJSBundleProvider extends JSBundleProvider {
  static fromServerIp(ip: string, port: number = 8081, appKeys: string[] = []): MetroJSBundleProvider {
    return new MetroJSBundleProvider(`http://${ip}:${port}/index.bundle?platform=harmony&dev=true&minify=false`, appKeys)
  }

  constructor(private bundleUrl: string = "http://localhost:8081/index.bundle?platform=harmony&dev=true&minify=false", private appKeys: string[] = []) {
    super()
  }

  getAppKeys() {
    return this.appKeys
  }

  getURL() {
    return this.bundleUrl
  }

  getHotReloadConfig(): HotReloadConfig | null {
    const urlObj = urlUtils.URL.parseURL(this.getURL());
    const pathParts = urlObj.pathname.split('/');
    const bundleEntry = pathParts[pathParts.length - 1];
    const port = urlObj.port ?? 8081;
    const scheme = urlObj.protocol.slice(0, -1);

    return {
      bundleEntry,
      host: urlObj.hostname,
      port,
      scheme,
    }
  }

  async getBundle(onProgress?: (progress: number) => void): Promise<ArrayBuffer> {
    try {
      const response = await fetchDataFromUrl(this.bundleUrl, { headers: { 'Content-Type': 'text/javascript' } }, onProgress);
      /**
       * When the responseCode is 500, instead of building a bundle, Metro will report an error which should be thrown and displayed on the phone.
       */
      if (response.responseCode === 500) {
        this.throwMetroError(response.result)
      }
      return response.result;
    } catch (err) {
      if (err instanceof MetroJSBundleProviderError) {
        throw err
      }
      const suggestions = ["Is Metro server running? Did you run `react-native start`?"]
      const hotReloadConfig = this.getHotReloadConfig()
      if (hotReloadConfig) {
        if (hotReloadConfig.host === "localhost") {
          suggestions.push(`Try forwarding data from a device port to a host port (hdc rport tcp:${hotReloadConfig.port} tcp:${hotReloadConfig.port})`)
          suggestions.push(`Are you testing on a real device? Did you connect it to your computer?`)
        } else {
          suggestions.push(`Are you testing on a real device? Is your phone connected to the same Wi-Fi network as your computer?`)
          suggestions.push(`Are you testing on a real device? Does "${hotReloadConfig.host}" matches your computer IP in your local network? Did you use "react-native create-metro-host-constants-harmony"?`)
        }
      }
      throw new JSBundleProviderError({
        whatHappened: `Couldn't load JSBundle from ${this.bundleUrl}`,
        extraData: err,
        howCanItBeFixed: suggestions
      })
    }
  }

  getHumanFriendlyURL(): string {
    const hotReloadConfig = this.getHotReloadConfig()!
    return `${hotReloadConfig.host}:${hotReloadConfig.port}`
  }

  throwMetroError(result: ArrayBuffer) {
    const processChunk = (text: string | undefined) => {
      let content = text ?? ""
      const matches = text?.match(/^([!\x3c-\x3f]*)([\d;]*)([\x20-\x2c]*[\x40-\x7e])([\s\S]*)/m)
      return !matches ? content : matches[4]
    }
    const ansiToText = (txt: string) => {
      const rawTextChunks = txt.split(/\033\[/);
      const firstChunk = rawTextChunks.shift() ?? "";
      const colorChunks = rawTextChunks.map(processChunk);
      colorChunks.unshift(firstChunk);
      return colorChunks.join("");
    }
    const textDecoder = util.TextDecoder.create();
    const err: { message: string, stack: string } = JSON.parse(textDecoder.decodeWithStream(new Uint8Array(result)));
    throw new MetroJSBundleProviderError({
      whatHappened: ansiToText(err.message),
      customStack: err.stack.replace(err.message, ""),
      howCanItBeFixed: []
    })
  }
}

export class AnyJSBundleProvider extends JSBundleProvider {
  private pickedJSBundleProvider: JSBundleProvider | undefined = undefined

  constructor(private jsBundleProviders: JSBundleProvider[]) {
    super()
    if (jsBundleProviders.length === 0) {
      throw new JSBundleProviderError({
        whatHappened: "AnyJSBundleProvider received an empty list of providers",
        howCanItBeFixed: ["Provide at least one JSBundleProvider to AnyJSBundleProvider"]
      })
    }
  }

  getURL() {
    const jsBundleProvider = this.pickedJSBundleProvider ?? this.jsBundleProviders[0]
    return jsBundleProvider?.getURL() ?? "?"
  }

  getHumanFriendlyURL(): string {
    const jsBundleProvider = this.pickedJSBundleProvider ?? this.jsBundleProviders[0]
    return jsBundleProvider?.getHumanFriendlyURL() ?? "?"
  }

  getAppKeys() {
    if (!this.pickedJSBundleProvider) {
      return []
    }
    return this.pickedJSBundleProvider.getAppKeys()
  }

  async getBundle(onProgress?: (progress: number) => void) {
    const errors: JSBundleProviderError[] = []
    for (const jsBundleProvider of this.jsBundleProviders) {
      try {
        const bundle = await jsBundleProvider.getBundle(onProgress)
        this.pickedJSBundleProvider = jsBundleProvider;
        return bundle;
      } catch (err) {
        if (err instanceof MetroJSBundleProviderError) {
          throw err
        }
        if (err instanceof JSBundleProviderError) {
          errors.push(err)
        }
      }
    }
    const reducedError = JSBundleProviderError.fromMultipleRNOHErrors(errors)
    throw new JSBundleProviderError({
      whatHappened: "None of the provided JSBundleProviders was able to load a bundle",
      howCanItBeFixed: reducedError.getSuggestions(),
      extraData: reducedError.getDetails()
    })
  }

  getHotReloadConfig(): HotReloadConfig | null {
    if (this.pickedJSBundleProvider) {
      return this.pickedJSBundleProvider.getHotReloadConfig()
    }
    return null

  }
}

export class TraceJSBundleProviderDecorator extends JSBundleProvider {
  private logger: RNOHLogger

  constructor(private jsBundleProvider: JSBundleProvider, logger: RNOHLogger) {
    super()
    this.logger = logger.clone('TraceJSBundleProviderDecorator')
  }

  getURL() {
    return this.jsBundleProvider.getURL()
  }

  async getBundle(onProgress?: (progress: number) => void) {
    const stopTracing = this.logger.clone('getBundle').startTracing()
    const result = await this.jsBundleProvider.getBundle(onProgress)
    stopTracing()
    return result
  }

  getAppKeys() {
    return this.jsBundleProvider.getAppKeys()
  }

  getHotReloadConfig(): HotReloadConfig {
    return this.jsBundleProvider.getHotReloadConfig()
  }

  getHumanFriendlyURL(): string {
    return this.jsBundleProvider.getHumanFriendlyURL()
  }
}