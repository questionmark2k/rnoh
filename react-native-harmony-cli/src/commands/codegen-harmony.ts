import fs from 'node:fs';
import pathUtils from 'node:path';
import { Command } from '@react-native-community/cli-types';
import {
  AbsolutePath,
  DescriptiveError,
  maybeMakeDirectories,
  maybeRemoveFilesInDirectory,
} from '../core';
import {
  Codegen,
  UberSchema,
  ComponentCodeGenerator,
  NativeModuleCodeGenerator,
  PackageCodeGenerator,
} from '../codegen';
import { Logger } from '../io';

export const commandCodegenHarmony: Command = {
  name: 'codegen-harmony',
  description:
    'Generates a glue layer and native utilities from Turbo Module or Fabric Components specifications',
  options: [
    {
      name: '--rnoh-module-path <path>',
      description: 'Specifies the relative path to the rnoh OHOS module',
    },
    {
      name: '--cpp-output-path <path>',
      description:
        'Specifies the relative path to the output directory intended for storing generated C++ files',
      default: './harmony/entry/src/main/cpp/generated',
    },
    {
      name: '--project-root-path <path>',
      description: 'Relative path to package root',
      default: '.',
    },
    {
      name: '--debug [boolean]',
      description: 'Enables logging details',
      default: false,
    },
  ],
  func: async (_argv, _config, args: any) => {
    const logger = new Logger();
    try {
      validateArgs(args);
      const etsOutputPath = new AbsolutePath(
        args.rnohModulePath
      ).copyWithNewSegment('generated');
      const cppOutputPath = new AbsolutePath(args.cppOutputPath);
      const projectRootPath = new AbsolutePath(args.projectRootPath);
      const codegen = createCodegen(cppOutputPath, etsOutputPath);
      const uberSchema = UberSchema.fromProjectRootPath(
        new AbsolutePath(args.projectRootPath)
      );
      const fileContentByPath = codegen.generate(uberSchema);
      if (args.debug) {
        logger.debug((styles) =>
          styles.gray(JSON.stringify(uberSchema.getValue(), null, 2))
        );
      }
      saveCodegenResult(fileContentByPath, cppOutputPath, etsOutputPath);
      logCodegenResult(logger, fileContentByPath, projectRootPath);
    } catch (err) {
      if (err instanceof DescriptiveError) {
        logger.descriptiveError(err);
        process.exit(1);
      }
      throw err;
    }
  },
};

function validateArgs(args: any) {
  if (!args.rnohModulePath) {
    throw new DescriptiveError({
      whatHappened: "--rnoh-module-path argument wasn't provided",
      whatCanUserDo: {
        default: [
          'Please provide a path to the native React Native for Open Harmony module (rnoh) which is probably located somewhere in "<PROJECT_ROOT>/harmony" directory',
        ],
      },
    });
  }
}

function createCodegen(
  cppOutputPath: AbsolutePath,
  etsOutputPath: AbsolutePath
) {
  return new Codegen(new PackageCodeGenerator(cppOutputPath, etsOutputPath), {
    Component: new ComponentCodeGenerator(
      cppOutputPath,
      etsOutputPath.copyWithNewSegment('components')
    ),
    NativeModule: new NativeModuleCodeGenerator(
      cppOutputPath,
      etsOutputPath.copyWithNewSegment('turboModules')
    ),
  });
}

function saveCodegenResult(
  fileContentByPath: Map<AbsolutePath, string>,
  cppOutputPath: AbsolutePath,
  etsOutputPath: AbsolutePath
) {
  prepareDirectory(cppOutputPath);
  prepareDirectory(etsOutputPath);
  prepareDirectory(etsOutputPath.copyWithNewSegment('components'));
  prepareDirectory(etsOutputPath.copyWithNewSegment('turboModules'));
  fileContentByPath.forEach((fileContent, path) => {
    fs.writeFileSync(path.getValue(), fileContent);
  });
}

function prepareDirectory(path: AbsolutePath) {
  maybeMakeDirectories(path);
  maybeRemoveFilesInDirectory(path);
}

function logCodegenResult(
  logger: Logger,
  fileContentByPath: Map<AbsolutePath, string>,
  projectRootPath: AbsolutePath
) {
  const sortedRelativePathStrings = Array.from(fileContentByPath.keys()).map(
    (path) => pathUtils.relative(projectRootPath.getValue(), path.getValue())
  );
  sortedRelativePathStrings.sort();
  sortedRelativePathStrings.forEach((pathStr) => {
    logger.info((styles) => styles.gray(`• ${pathStr}`));
  });
  logger.info(() => '');
  logger.info(
    (styles) =>
      `Generated ${styles.green(styles.bold(fileContentByPath.size))} file(s)`,
    { prefix: true }
  );
  logger.info(() => '');
}
