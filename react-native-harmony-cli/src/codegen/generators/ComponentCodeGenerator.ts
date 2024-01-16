import { AbsolutePath } from '../../core';
import type { SpecCodeGenerator } from '../core/Codegen';
import { SpecSchema } from '../core/Schema';

export class ComponentCodeGenerator implements SpecCodeGenerator {
  generate(schema: SpecSchema): Map<AbsolutePath, string> {
    // TODO: implement
    return new Map();
  }
}
