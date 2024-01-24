import { AbsolutePath } from '../../core';
import fs from 'fs';
import { CodegenConfig } from './CodegenConfig';
import { CodegenError } from './CodegenError';

export class PackageJSON {
  static fromProjectRootPath(
    packageRootPath: AbsolutePath,
    projectRootPath: AbsolutePath
  ) {
    const packageJSONPath = packageRootPath.copyWithNewSegment('package.json');
    if (!fs.existsSync(packageJSONPath.getValue())) {
      throw new CodegenError({
        whatHappened: "Couldn't find 'package.json'",
        whatCanUserDo: {
          default: [
            'Check if the path to project root is correct',
            'Try changing working directory to project root',
          ],
        },
        extraData: packageJSONPath.getValue(),
      });
    }
    return new PackageJSON(
      JSON.parse(fs.readFileSync(packageJSONPath.getValue()).toString()),
      packageRootPath,
      projectRootPath
    );
  }

  private constructor(
    private rawPackageJSON: Record<string, any>,
    private packageRootPath: AbsolutePath,
    private projectRootPath: AbsolutePath
  ) {}

  getName(): string {
    return this.rawPackageJSON.name;
  }

  maybeCreateCodegenConfig(): CodegenConfig | null {
    /**
     * Shared codegenConfig is not used on purpose. Android, iOS and other platforms shouldn't
     * generate code for harmony-specific packages. The shape of this codegenConfig is different
     * that the shape of the shared config, to allow specifying which specific turbo modules or
     * fabric components should be code generated.
     */
    const rawCodegenConfig = this.rawPackageJSON.harmony?.codegenConfig;
    if (!rawCodegenConfig) {
      return null;
    }
    return CodegenConfig.fromRawCodegenConfig(
      rawCodegenConfig,
      this.packageRootPath,
      this.projectRootPath,
      this.getName()
    );
  }
}
