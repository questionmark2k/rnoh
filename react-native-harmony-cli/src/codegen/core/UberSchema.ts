import {
  ComponentSchema,
  NativeModuleSchema,
  SchemaType as RawUberSchema,
} from '@react-native/codegen/lib/CodegenSchema';
import { AbsolutePath, ValueObject } from '../../core';
import { PackageJSON } from './PackageJSON';
import { CodegenConfig } from './CodegenConfig';
import { ProjectDependenciesManager } from './ProjectDependenciesManager';
// @ts-expect-error
import extractUberSchemaFromSpecFilePaths_ from '@react-native/codegen/lib/cli/combine/combine-js-to-schema.js';
import { CodegenError } from './CodegenError';

function createRawUberSchemaFromSpecFilePaths(
  projectSourceFilePaths: AbsolutePath[]
): RawUberSchema {
  return extractUberSchemaFromSpecFilePaths_(
    projectSourceFilePaths.map((p) => p.getValue())
  );
}

export type SpecSchema = ComponentSchema | NativeModuleSchema;
export type SpecSchemaType = SpecSchema['type'];
type FindSpecSchemaByType<
  TType extends SpecSchemaType,
  TSpecSchema = SpecSchema
> = TSpecSchema extends { type: TType } ? TSpecSchema : never;

export class UberSchema implements ValueObject {
  static fromProjectRootPath(projectRootPath: AbsolutePath): UberSchema {
    const packageJSON = PackageJSON.fromProjectRootPath(
      projectRootPath,
      projectRootPath
    );
    const codegenConfigs: CodegenConfig[] = [];
    const appCodegenConfig = packageJSON.maybeCreateCodegenConfig();
    if (appCodegenConfig) {
      codegenConfigs.push(appCodegenConfig);
    }
    new ProjectDependenciesManager(projectRootPath).forEach((dependency) => {
      const codegenConfig = dependency.maybeCreateCodegenConfig();
      if (codegenConfig) {
        codegenConfigs.push(codegenConfig);
      }
    });
    try {
      return new UberSchema(
        createRawUberSchemaFromSpecFilePaths(
          codegenConfigs.flatMap((codegenConfig) =>
            codegenConfig.getSpecFilePaths()
          )
        )
      );
    } catch (err) {
      if (err instanceof Error) {
        throw new CodegenError({
          whatHappened: "Couldn't create the schema",
          whatCanUserDo: {
            default: [
              `There's probably at least one spec file defined in your project or in a third-party package that breaks some code generation restrictions. Please check the message below. If it's ambiguous, debug the problem with divide and conquer strategy.\n\n${err.message}`,
            ],
          },
        });
      }
      throw err;
    }
  }

  private constructor(private schemaValue: RawUberSchema) {}

  getValue(): RawUberSchema {
    return this.schemaValue;
  }

  findAllSpecSchemasByType<TSpecSchemaType extends SpecSchemaType>(
    schemaType: TSpecSchemaType
  ): FindSpecSchemaByType<TSpecSchemaType>[] {
    return Object.values(this.schemaValue.modules)
      .filter((module) => module.type === schemaType)
      .map((module) => module as FindSpecSchemaByType<TSpecSchemaType>);
  }

  getSpecSchemaByFilenameMap(): ReadonlyMap<string, SpecSchema> {
    return Object.entries(this.schemaValue.modules).reduce(
      (acc, [moduleName, moduleSchema]) => {
        acc.set(moduleName, moduleSchema);
        return acc;
      },
      new Map<string, SpecSchema>()
    );
  }
}
