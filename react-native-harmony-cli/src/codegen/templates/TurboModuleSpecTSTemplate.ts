import mustache from 'mustache';

const TEMPLATE = `
// This file was generated.
import { Tag } from "../../ts"

export namespace {{name}} {
  export const NAME = '{{name}}' as const

  {{#enums}}
  export enum {{name}} {
    {{#members}}
    {{name}} = {{{value}}},
    {{/members}}
  }

  {{/enums}}
  {{#aliases}}
  export type {{name}} = {{{type}}}
  
  {{/aliases}}
  export interface Spec {
    {{#methods}}
    {{name}}({{{stringifiedArgs}}}): {{{returnType}}};
  
    {{/methods}}
  }
}
`;

type Alias = {
  name: string;
  type: string;
};

type EnumModel = {
  name: string;
  members: {
    name: string;
    value: string;
  }[];
};

type Method = {
  name: string;
  stringifiedArgs: string;
  returnType: string;
};

export class TurboModuleInterfaceTS {
  private methods: Method[] = [];
  private aliases: Alias[] = [];
  private enumModels: EnumModel[] = [];

  constructor(private name: string) {}

  addAlias(alias: Alias) {
    this.aliases.push(alias);
  }

  addEnum(enumModel: EnumModel) {
    this.enumModels.push(enumModel);
  }

  addMethod(method: Method) {
    this.methods.push(method);
  }

  build(): string {
    return mustache.render(TEMPLATE.trimStart(), {
      name: this.name,
      aliases: this.aliases,
      enums: this.enumModels,
      methods: this.methods.map((method) => ({
        name: method.name,
        stringifiedArgs: method.stringifiedArgs,
        returnType: method.returnType,
      })),
    });
  }
}
