import { AbsolutePath } from '../../core';
import {
  CodeGenerator,
  UberSchema,
  createEventNameFromEventHandlerName,
} from '../core';
import { IndexTSTemplate, RNOHGeneratedPackageHTemplate } from '../templates';

export class PackageCodeGenerator implements CodeGenerator<UberSchema> {
  constructor(
    private cppOutputPath: AbsolutePath,
    private etsOutputPath: AbsolutePath
  ) {}

  generate(uberSchema: UberSchema): Map<AbsolutePath, string> {
    const result = new Map<AbsolutePath, string>();
    const generatedPackageH = new RNOHGeneratedPackageHTemplate();
    const components_indexTS = new IndexTSTemplate();
    const turboModules_indexTS = new IndexTSTemplate();
    const generated_indexETS = new IndexTSTemplate();
    generated_indexETS.addReexport('ts');

    uberSchema
      .findAllSpecSchemasByType('NativeModule')
      .forEach((nativeModuleSchema) => {
        generatedPackageH.addTurboModule({
          name: nativeModuleSchema.moduleName,
        });
        turboModules_indexTS.addReexport(`${nativeModuleSchema.moduleName}`);
      });
    uberSchema
      .findAllSpecSchemasByType('Component')
      .forEach((componentSchema) => {
        Object.entries(componentSchema.components).forEach(
          ([name, componentShape]) => {
            components_indexTS.addReexport(name);
            generatedPackageH.addComponent({
              name: name,
              supportedEventNames: componentShape.events.map((e) => {
                return createEventNameFromEventHandlerName(e.name);
              }),
            });
          }
        );
      });
    result.set(
      this.cppOutputPath.copyWithNewSegment('RNOHGeneratedPackage.h'),
      generatedPackageH.build()
    );
    result.set(
      this.etsOutputPath.copyWithNewSegment('ts.ts'),
      new IndexTSTemplate()
        .addReexport('components/ts')
        .addReexport('turboModules/ts')
        .build()
    );
    result.set(
      this.etsOutputPath.copyWithNewSegment('components', 'ts.ts'),
      components_indexTS.build()
    );
    result.set(
      this.etsOutputPath.copyWithNewSegment('turboModules', 'ts.ts'),
      turboModules_indexTS.build()
    );
    result.set(
      this.etsOutputPath.copyWithNewSegment('index.ets'),
      generated_indexETS.build()
    );
    return result;
  }
}
