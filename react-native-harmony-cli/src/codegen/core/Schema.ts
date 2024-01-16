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

function createRawUberSchemaFromSpecFilePaths(
  projectSourceFilePaths: AbsolutePath[]
): RawUberSchema {
  return extractUberSchemaFromSpecFilePaths_(
    projectSourceFilePaths.map((p) => p.getValue())
  );
}

export type SpecSchema = ComponentSchema | NativeModuleSchema;
export type SpecSchemaType = SpecSchema['type'];

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
    return new UberSchema(
      createRawUberSchemaFromSpecFilePaths(
        codegenConfigs.flatMap((codegenConfig) =>
          codegenConfig.getSpecFilePaths()
        )
      )
    );
  }

  private constructor(private schemaValue: RawUberSchema) {}

  getValue(): RawUberSchema {
    return this.schemaValue;
  }

  getTurboModuleNames(): string[] {
    return Object.values(this.schemaValue.modules)
      .filter((module) => module.type === 'NativeModule')
      .map((module) => (module as NativeModuleSchema).moduleName);
  }

  getSchemaByFileName(): ReadonlyMap<string, SpecSchema> {
    return Object.entries(this.schemaValue.modules).reduce(
      (acc, [moduleName, moduleSchema]) => {
        acc.set(moduleName, moduleSchema);
        return acc;
      },
      new Map<string, SpecSchema>()
    );
  }
}
