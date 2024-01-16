import fs from 'node:fs';
import pathUtils from 'node:path';
import { Command } from '@react-native-community/cli-types';
import { AbsolutePath, maybeMkDirs } from '../core';
import { Logger } from '../io';
import {
  Codegen,
  UberSchema,
  ComponentCodeGenerator,
  NativeModuleCodeGenerator,
  PackageCodeGenerator,
} from '../codegen';
import { CodegenError } from '../codegen/core/CodegenError';

export const commandCodegenHarmony: Command = {
  name: 'codegen-harmony',
  description:
    '[Experimental] Generates native code from Turbo Module or Fabric Components Specifications',
  options: [
    {
      name: '--ets-output-path <path>',
      description:
        'Specifies the relative path to the output directory intended for storing generated eTS or TS files',
      default: './harmony/entry/src/main/ets/generated',
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
  ],
  func: async (argv, config, args: any) => {
    const logger = new Logger();
    try {
      const cppOutputPath = new AbsolutePath(args.cppOutputPath);
      const projectRootPath = new AbsolutePath(args.projectRootPath);
      const codegen = new Codegen(new PackageCodeGenerator(cppOutputPath), {
        Component: new ComponentCodeGenerator(),
        NativeModule: new NativeModuleCodeGenerator(cppOutputPath),
      });
      const schema = UberSchema.fromProjectRootPath(
        new AbsolutePath(args.projectRootPath)
      );
      const fileContentByPath = codegen.generate(schema);
      maybeMkDirs(cppOutputPath);
      fileContentByPath.forEach((fileContent, path) => {
        fs.writeFileSync(path.getValue(), fileContent);
      });

      fileContentByPath.forEach((_fileContent, path) => {
        logger.info((styles) =>
          styles.gray(
            `• ${pathUtils.relative(
              projectRootPath.getValue(),
              path.getValue()
            )}`
          )
        );
      });
      logger.info(() => '');
      logger.info(
        (styles) =>
          `Generated ${styles.green(
            styles.bold(fileContentByPath.size)
          )} file(s)`,
        { prefix: true }
      );
      logger.info(() => '');
    } catch (err) {
      if (err instanceof CodegenError) {
        logger.descriptiveError(err);
        process.exit(1);
      }
      throw err;
    }
  },
};
