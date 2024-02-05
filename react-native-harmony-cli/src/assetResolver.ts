import type { AssetData } from 'metro';

export type Asset = AssetData;

/*
    cyrb53 (c) 2018 bryc (github.com/bryc)
    License: Public domain. Attribution appreciated.
    A fast and simple 53-bit string hash function with decent collision resistance.
    Largely inspired by MurmurHash2/3, but with a focus on speed/simplicity.
*/
function cyrb53(str: string, seed = 0) {
  let h1 = 0xdeadbeef ^ seed, h2 = 0x41c6ce57 ^ seed;
  for(let i = 0, ch; i < str.length; i++) {
    ch = str.charCodeAt(i);
    h1 = Math.imul(h1 ^ ch, 2654435761);
    h2 = Math.imul(h2 ^ ch, 1597334677);
  }
  h1  = Math.imul(h1 ^ (h1 >>> 16), 2246822507);
  h1 ^= Math.imul(h2 ^ (h2 >>> 13), 3266489909);
  h2  = Math.imul(h2 ^ (h2 >>> 16), 2246822507);
  h2 ^= Math.imul(h1 ^ (h1 >>> 13), 3266489909);
  return 4294967296 * (2097151 & h2) + (h1 >>> 0);
};


export function getAssetDestRelativePath(asset: Asset): string {
  const fileName = getResourceIdentifier(asset);
  return `${fileName}.${asset.type}`;
}

function getResourceIdentifier(asset: Asset): string {
  const folderPath = getBasePath(asset);
  const fullAssetPath = `${folderPath}/${asset.name}.${asset.type}`.toLowerCase();
  const assetPathHash = cyrb53(fullAssetPath);
  return assetPathHash.toString(16);
}

function getBasePath(asset: Asset): string {
  let basePath = asset.httpServerLocation;
  if (basePath[0] === '/') {
    basePath = basePath.substr(1);
  }
  return basePath;
}
