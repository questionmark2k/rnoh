export class RNOHError extends Error {
  static fromMultipleRNOHErrors(rnohErrors: RNOHError[]) {
    const message = rnohErrors.map((err, idx) => {
      return `${idx + 1}) ${err.getMessage()}`
    }).join("\n")
    const suggestions = rnohErrors.flatMap(err => err.getSuggestions())
    const details = rnohErrors.map(err => err.getDetails()).filter(d =>!!d).map((d, idx) => `${idx + 1}) ${d.trim()}`).join("\n\n")
    return new RNOHError({ whatHappened: message, howCanItBeFixed: suggestions, extraData: details })
  }

  constructor(private data: {
    whatHappened: string,
    howCanItBeFixed: string[]
    extraData?: any
    customStack?: string
  }) {
    super(data.whatHappened)
  }

  getMessage() {
    return this.data.whatHappened
  }

  getSuggestions() {
    return this.data.howCanItBeFixed
  }

  getStack() {
    return this.data.customStack ?? this.stack
  }

  getDetails(): string {
    if (!this.data.extraData) {
      return ""
    }
    if (this.data.extraData instanceof Error) {
      return this.data.extraData.message.trim()
    }
    if (typeof this.data.extraData === "object" && "message" in this.data.extraData) {
      return this.data.extraData.message.trim()
    }
    if (typeof this.data.extraData === "string") {
      return this.data.extraData.trim()
    }
    try {
      return JSON.stringify(this.data.extraData, null, 2)
    } catch (err) {
      return ""
    }
  }
}

export class FatalRNOHError extends RNOHError {
}