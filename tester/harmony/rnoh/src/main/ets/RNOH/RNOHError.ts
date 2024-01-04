export class RNOHError extends Error {
  static fromMultipleRNOHErrors(rnohErrors: RNOHError[]) {
    const message = rnohErrors.map((err, idx) => {
      return `${idx + 1}) ${err.getMessage()}`
    }).join("\n")
    const suggestions = rnohErrors.flatMap(err => err.getSuggestions())
    const details = rnohErrors.map(err => err.getDetails()).filter(d =>!!d).map((d, idx) => `${idx + 1}) ${d}`).join("\n")
    return new RNOHError({ whatHappened: message, howCanItBeFixed: suggestions, originalErrorOrContextData: details })
  }

  constructor(private data: {
    whatHappened: string,
    howCanItBeFixed: string[]
    originalErrorOrContextData?: any
  }) {
    super(data.whatHappened)
  }

  getMessage() {
    return this.data.whatHappened
  }

  getSuggestions() {
    return this.data.howCanItBeFixed
  }

  getDetails(): string {
    if (!this.data.originalErrorOrContextData) {
      return ""
    }
    if (this.data.originalErrorOrContextData instanceof Error) {
      return this.data.originalErrorOrContextData.message
    }
    if (typeof this.data.originalErrorOrContextData === "object" && "message" in this.data.originalErrorOrContextData) {
      return this.data.originalErrorOrContextData.message
    }
    if (typeof this.data.originalErrorOrContextData === "string") {
      return this.data.originalErrorOrContextData
    }
    return ""
  }
}