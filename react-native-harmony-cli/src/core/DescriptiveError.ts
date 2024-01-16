export class DescriptiveError extends Error {
  constructor(
    private data:
      | {
          whatHappened: string;
          whatCanUserDo: {
            default?: string[];
            rnAppDeveloper?: string[];
            rnPackageDeveloper?: string[];
          };
          extraData?: any;
        }
      | {
          whatHappened: string;
          unexpected: true;
          extraData?: any;
        }
  ) {
    super(data.whatHappened);
  }

  isUnexpected(): boolean {
    return 'unexpected' in this.data;
  }

  getMessage(): string {
    return this.data.whatHappened;
  }

  getSuggestionsByRole() {
    if ('whatCanUserDo' in this.data) {
      return this.data.whatCanUserDo;
    } else {
      return null;
    }
  }

  getDetails(): string {
    if (!this.data.extraData) {
      return '';
    }
    if (typeof this.data.extraData === 'string') {
      return this.data.extraData.trim();
    }
    if (this.data.extraData instanceof Error) {
      let lines = [
        `${this.data.extraData.name}: ${this.data.extraData.message}`,
      ];
      for (let stackEntry of (this.data.extraData.stack ?? '').split('\n')) {
        lines.push('');
        lines.push(stackEntry);
      }
      return lines.join('\n');
    }
    try {
      return JSON.stringify(this.data.extraData, null, 2);
    } catch (err) {
      return '';
    }
  }
}
