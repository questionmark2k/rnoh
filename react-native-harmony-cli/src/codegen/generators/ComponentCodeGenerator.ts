import { AbsolutePath } from '../../core';
import {
  CodegenError,
  TypeAnnotationToTS,
  createEventNameFromEventHandlerName,
  getStringifiedTSDefaultValueFromPropTypeAnnotation,
} from '../core';
import type { SpecCodeGenerator } from '../core';
import { SpecSchema } from '../core';
import {
  ComponentDescriptorHTemplate,
  ComponentJSIBinderHTemplate,
  ComponentUtilsTSTemplate,
} from '../templates';

export class ComponentCodeGenerator implements SpecCodeGenerator {
  constructor(
    private cppOutputPath: AbsolutePath,
    private tsOutputPath: AbsolutePath
  ) {}

  generate(schema: SpecSchema): Map<AbsolutePath, string> {
    if (schema.type !== 'Component') {
      throw new CodegenError({
        whatHappened: `ComponentCodeGenerator can't generate code for a module type: ${schema.type}`,
        unexpected: true,
      });
    }
    const typeAnnotationToTS = new TypeAnnotationToTS();
    const result = new Map<AbsolutePath, string>();
    Object.entries(schema.components).map(([name, shape]) => {
      const componentDescriptorHTemplate = new ComponentDescriptorHTemplate(
        name
      );
      const componentJSIBinderHTemplate = new ComponentJSIBinderHTemplate(name);
      const componentUtilsTSTemplate = new ComponentUtilsTSTemplate(name);

      shape.props.forEach((prop) => {
        componentJSIBinderHTemplate.addProp({
          name: prop.name,
          isObject: prop.typeAnnotation.type === 'ObjectTypeAnnotation',
        });

        componentUtilsTSTemplate.addProp({
          name: prop.name,
          tsType: typeAnnotationToTS.convert(prop.typeAnnotation),
          defaultValue: getStringifiedTSDefaultValueFromPropTypeAnnotation(
            prop.typeAnnotation
          ),
          isOptional: prop.optional,
        });
      });

      shape.commands.forEach((command) => {
        componentUtilsTSTemplate.addCommand({
          name: command.name,
          argsTSType:
            '[' +
            command.typeAnnotation.params
              .map(
                (param) =>
                  typeAnnotationToTS.convert(param.typeAnnotation) +
                  (param.optional ? '?' : '')
              )
              .join(', ') +
            ']',
        });
      });

      shape.events.forEach((event) => {
        componentUtilsTSTemplate.addEvent({
          name: createEventNameFromEventHandlerName(event.name),
          payloadTSType: event.typeAnnotation.argument
            ? typeAnnotationToTS.convert(event.typeAnnotation.argument)
            : 'undefined',
        });
        componentJSIBinderHTemplate.addEvent({
          type: event.bubblingType,
          eventHandlerName: event.name,
        });
      });

      result.set(
        this.cppOutputPath.copyWithNewSegment(`${name}ComponentDescriptor.h`),
        componentDescriptorHTemplate.build()
      );
      result.set(
        this.cppOutputPath.copyWithNewSegment(`${name}JSIBinder.h`),
        componentJSIBinderHTemplate.build()
      );
      result.set(
        this.tsOutputPath.copyWithNewSegment(`${name}.ts`),
        componentUtilsTSTemplate.build()
      );
    });
    return result;
  }
}
