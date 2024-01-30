import {
  getAssetDestRelativePath,
  Asset,
} from '@rnoh/react-native-harmony-cli/src/assetResolver';
import {
  getBasePath,
} from '@react-native/assets-registry/path-support';
import { Platform } from 'react-native';

type ResolvedAssetSource = {
  readonly __packager_asset: boolean;
  readonly width?: number;
  readonly height?: number;
  readonly uri: string;
  readonly scale: number;
};

function getAssetPath(asset: Asset): string {
  const assetDir = getBasePath(asset);
  return assetDir + '/' + asset.name + '.' + asset.type;
}

class AssetSourceResolver {
  constructor(
    private serverUrl: string | undefined,
    private jsbundleUrl: string | undefined,
    private asset: Asset
  ) {}

  isLoadedFromServer(): boolean {
    return !!this.serverUrl;
  }

  public defaultAsset(): ResolvedAssetSource {
    if (this.isLoadedFromServer()) {
      return this.assetServerURL();
    }

    return {
      __packager_asset: this.asset.__packager_asset,
      uri: `asset://${getAssetDestRelativePath(this.asset)}`,
      scale: 1,
      width: this.asset.width,
      height: this.asset.height,
    };
  }

  /**
   * Returns an absolute URL which can be used to fetch the asset
   * from the devserver
   */
  assetServerURL(): ResolvedAssetSource {
    if (!this.serverUrl) {
      throw new Error('need server to load from');
    }

    return this.fromSource(
      this.serverUrl +
      getAssetPath(this.asset) +
        '?platform=' +
        Platform.OS +
        '&hash=' +
        this.asset.hash,
    );
  }

  fromSource(source: string): ResolvedAssetSource {
    return {
      __packager_asset: true,
      width: this.asset.width,
      height: this.asset.height,
      uri: source,
      scale: 1,
    };
  }
}

module.exports = AssetSourceResolver;
