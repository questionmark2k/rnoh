import mustache from 'mustache';

const TEMPLATE = `
// This file was generated.
{{#components}}
export * from "./{{{name}}}"
{{/components}}
`;

export class IndexTSTemplate {
  private importName: string[] = [];

  constructor() {}

  addReexport(name: string) {
    this.importName.push(name);
    return this;
  }

  build(): string {
    return mustache.render(TEMPLATE.trimStart(), {
      components: this.importName.map((name) => ({ name })),
    });
  }
}
