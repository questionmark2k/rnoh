import fs from 'fs';
import { AbsolutePath } from './AbsolutePath';
import { CodegenConfig } from './CodegenConfig';
import { DescriptiveError } from './DescriptiveError';

export class PackageJSON {
  static fromProjectRootPath(
    packageRootPath: AbsolutePath,
    projectRootPath: AbsolutePath
  ) {
    const packageJSONPath = packageRootPath.copyWithNewSegment('package.json');
    if (!fs.existsSync(packageJSONPath.getValue())) {
      throw new DescriptiveError({
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

  get name(): string {
    return this.rawPackageJSON.name;
  }

  get version(): string {
    return this.rawPackageJSON.version;
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
      this.name
    );
  }
}
