import { AbsolutePath } from '../../core';
import type { SpecCodeGenerator } from '../core/Codegen';
import { CodegenError } from '../core/CodegenError';
import { TurboModuleCppTemplate, TurboModuleHTemplate } from '../templates';
import { SpecSchema } from '../core/Schema';

export class NativeModuleCodeGenerator implements SpecCodeGenerator {
  constructor(private cppOutputPath: AbsolutePath) {}

  generate(schema: SpecSchema): Map<AbsolutePath, string> {
    const result = new Map<AbsolutePath, string>();
    if (schema.type !== 'NativeModule') {
      throw new CodegenError({
        whatHappened: `NativeModuleCodeGenerator can't generate code for module type: ${schema.type}`,
        unexpected: true,
      });
    }

    const turboModuleH = new TurboModuleHTemplate(schema.moduleName).build();
    result.set(
      this.cppOutputPath.copyWithNewSegment(`${schema.moduleName}.h`),
      turboModuleH
    );

    const turboModuleCpp = new TurboModuleCppTemplate(schema.moduleName);
    schema.spec.properties.map((prop) => {
      if (prop.typeAnnotation.type === 'FunctionTypeAnnotation') {
        turboModuleCpp.addMethod({
          name: prop.name,
          argsCount: prop.typeAnnotation.params.length,
          isAsync:
            prop.typeAnnotation.returnTypeAnnotation.type ===
            'PromiseTypeAnnotation',
        });
      }
    });
    result.set(
      this.cppOutputPath.copyWithNewSegment(`${schema.moduleName}.cpp`),
      turboModuleCpp.build()
    );

    return result;
  }
}
