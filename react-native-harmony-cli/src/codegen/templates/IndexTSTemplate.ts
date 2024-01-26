import mustache from 'mustache';

const TEMPLATE = `
// This file was generated.
{{#reexports}}
export * {{as}}from "{{{from}}}"
{{/reexports}}
`;

type Reexport = {
  from: string;
  as?: string;
};

export class IndexTSTemplate {
  private reexports: Reexport[] = [];

  constructor() {}

  addReexport(reexport: Reexport) {
    this.reexports.push(reexport);
    return this;
  }

  build(): string {
    return mustache.render(TEMPLATE.trimStart(), {
      reexports: this.reexports.map((reexport) => ({
        from: reexport.from,
        as: reexport.as ? `as ${reexport.as} ` : '',
      })),
    });
  }
}
