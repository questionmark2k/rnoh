import { AbsolutePath } from '../../core';
import { CodeGenerator, UberSchema } from '../core';
import { RNOHGeneratedPackageHTemplate } from '../templates';

export class PackageCodeGenerator implements CodeGenerator<UberSchema> {
  constructor(private cppOutputPath: AbsolutePath) {}

  generate(uberSchema: UberSchema): Map<AbsolutePath, string> {
    const result = new Map<AbsolutePath, string>();
    const template = new RNOHGeneratedPackageHTemplate();
    uberSchema.getTurboModuleNames().forEach((turboModuleName) => {
      template.addTurboModule({ name: turboModuleName });
    });
    result.set(
      this.cppOutputPath.copyWithNewSegment('RNOHGeneratedPackage.h'),
      template.build()
    );
    return result;
  }
}
