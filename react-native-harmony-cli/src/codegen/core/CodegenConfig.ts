import fs from 'node:fs';
import { AbsolutePath } from '../../core';
import { findFilePathsWithExtensions } from '../../io';
import { CodegenError } from './CodegenError';

export type RawCodegenConfig = {
  specPaths: string[];
};

export class CodegenConfig {
  static fromRawCodegenConfig(
    rawCodegenConfig: RawCodegenConfig,
    packageRootPath: AbsolutePath,
    projectRootPath: AbsolutePath,
    packageName: string
  ) {
    const SUPPORTED_SPEC_EXTENSIONS = ['js', 'jsx', 'ts', 'tsx'];
    const specPaths = (rawCodegenConfig.specPaths ?? []).map((rawSpecPath) => {
      return packageRootPath.copyWithNewSegment(
        rawSpecPath.replace('~', projectRootPath.getValue())
      );
    });
    const specFilePaths = specPaths.flatMap((specPath) => {
      if (!fs.existsSync(specPath.getValue())) {
        throw new CodegenError({
          whatHappened: `No such file or directory: ${specPath.getValue()}`,
          whatCanUserDo: {
            default: [
              `Please verify in "${packageName}/package.json::harmony::codegenConfig". If you are not the maintainer of this package, please report this problem to the package maintainer.`,
            ],
          },
        });
      }
      if (fs.lstatSync(specPath.getValue()).isDirectory()) {
        return findFilePathsWithExtensions(specPath, SUPPORTED_SPEC_EXTENSIONS);
      } else {
        const ext = specPath.getExtension();
        if (ext && SUPPORTED_SPEC_EXTENSIONS.includes(ext)) {
          return [specPath];
        }
        return [];
      }
    });
    return new CodegenConfig(specFilePaths);
  }

  private constructor(private specFilePaths: AbsolutePath[]) {}

  getSpecFilePaths(): AbsolutePath[] {
    return this.specFilePaths;
  }
}
