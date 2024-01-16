import { AbsolutePath } from '../../core';
import { CodegenError } from './CodegenError';
import { SpecSchema, SpecSchemaType, UberSchema } from './Schema';

export interface CodeGenerator<T> {
  generate(data: T): Map<AbsolutePath, string>;
}

export interface SpecCodeGenerator extends CodeGenerator<SpecSchema> {}

export class Codegen {
  constructor(
    private packageCodeGenerator: CodeGenerator<UberSchema>,
    private codeGeneratorBySchemaType: Record<SpecSchemaType, SpecCodeGenerator>
  ) {}

  generate(uberSchema: UberSchema): Map<AbsolutePath, string> {
    const result = new Map<AbsolutePath, string>();
    for (const [_fileName, schema] of uberSchema
      .getSchemaByFileName()
      .entries()) {
      if (!(schema.type in this.codeGeneratorBySchemaType)) {
        throw new CodegenError({
          whatHappened: `Could not find a code generator for a module type '${schema.type}'`,
          unexpected: true,
        });
      }
      this.codeGeneratorBySchemaType[schema.type]
        .generate(schema)
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
