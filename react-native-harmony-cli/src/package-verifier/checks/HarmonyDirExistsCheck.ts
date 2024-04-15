import fs from 'node:fs';
import { AbsolutePath } from '../../core';
import { Check } from '../types';

export function createHarmonyDirExistsCheck(
  packagePath: AbsolutePath,
  hasNativeCode: boolean
): Check {
  return {
    checkIf: 'package_has_harmony_dir',
    run: async () => {
      const harmonyDirExist = fs.existsSync(
        packagePath.copyWithNewSegment('harmony').getValue()
      );
      if (!hasNativeCode) {
        return 'skip';
      }
      if (!harmonyDirExist) {
        return 'fail';
      }
      return 'pass';
    },
  };
}
