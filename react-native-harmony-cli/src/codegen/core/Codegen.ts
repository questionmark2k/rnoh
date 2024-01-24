import { AbsolutePath } from '../../core';
import { CodegenError } from './CodegenError';
import { SpecSchema, SpecSchemaType, UberSchema } from './UberSchema';

export interface CodeGenerator<T> {
  generate(data: T): Map<AbsolutePath, string>;
}

export interface SpecCodeGenerator extends CodeGenerator<SpecSchema> {}

export class Codegen {
  constructor(
    private packageCodeGenerator: CodeGenerator<UberSchema>,
    private codeGeneratorBySpecSchemaType: Record<
      SpecSchemaType,
      SpecCodeGenerator
    >
  ) {}

  generate(uberSchema: UberSchema): Map<AbsolutePath, string> {
    const result = new Map<AbsolutePath, string>();
    for (const [_fileName, specSchema] of uberSchema
      .getSpecSchemaByFilenameMap()
      .entries()) {
      if (!(specSchema.type in this.codeGeneratorBySpecSchemaType)) {
        throw new CodegenError({
          whatHappened: `Could not find a code generator for a specification of type '${specSchema.type}'`,
          unexpected: true,
        });
      }
      this.codeGeneratorBySpecSchemaType[specSchema.type]
        .generate(specSchema)
        .forEach((fileContent, filePath) => {
          result.set(filePath, fileContent);
        });
    }
    this.packageCodeGenerator
      .generate(uberSchema)
      .forEach((fileContent, filePath) => {
        result.set(filePath, fileContent);
      });
    return result;
  }
}
