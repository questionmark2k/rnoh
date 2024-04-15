import json5 from 'json5';
import fs from 'node:fs';
import { AbsolutePath } from './AbsolutePath';

export class OHPackageJSON5 {
  static fromPath(ohPackageJson5Path: AbsolutePath) {
    return new OHPackageJSON5(
      json5.parse(
        fs.readFileSync(ohPackageJson5Path.getValue(), { encoding: 'utf-8' })
      )
    );
  }

  private constructor(private rawData: Record<string, any>) {}

  get name(): string {
    return this.rawData['name'];
  }

  get version(): string {
    return this.rawData['version'];
  }
}
