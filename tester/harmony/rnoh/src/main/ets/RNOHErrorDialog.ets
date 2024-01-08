import { RNAbility, Edges, RNOHError, FatalRNOHError, createSafeAreaInsets } from "./RNOH"


@Component
export struct RNOHErrorDialog {
  public controller!: CustomDialogController
  private isOpened: boolean = false
  private cleanUpListeners: (() => void)[] = []
  @StorageLink('RNAbility') private rnAbility: RNAbility | undefined = undefined

  aboutToAppear(): void {
    if (!this.rnAbility) {
      console.error("#RNOH", "RNOHErrorDialog couldn't find RNAbility. Did your Ability extends RNAbility?")
      return;
    }
    this.controller = new CustomDialogController({
      cornerRadius: 0,
      customStyle: true,
      cancel: () => {
        this.isOpened = false
      },
      alignment: DialogAlignment.TopStart,
      backgroundColor: Color.Transparent,
      builder: RNOHErrorCustomDialog({
        rnAbility: this.rnAbility!,
        onClose: () => {
          this.isOpened = false
        }
      })
    })
    this.cleanUpListeners.push(this.rnAbility!.eventEmitter.subscribe("NEW_ERROR", (err) => {
      if (!this.isOpened) {
        this.controller.open()
        this.isOpened = true
      }
    }))
    this.cleanUpListeners.push(this.rnAbility.devToolsController.eventEmitter.subscribe("DISMISS_RNOH_ERROR_DIALOG", () => {
      this.controller.close()
      this.isOpened = false
    }))
  }

  aboutToDisappear(): void {
    this.cleanUpListeners.forEach(cb => cb())
  }

  build() {
  }
}

const GUTTER = 16

@CustomDialog
struct RNOHErrorCustomDialog {
  public controller!: CustomDialogController
  public rnAbility!: RNAbility
  public onClose: () => void = () => {
  }
  private cleanUpListeners: (() => void)[] = []
  @State private insets: Edges<number> = { top: 0, left: 0, right: 0, bottom: 0 }
  @State private lastError: RNOHError | null = null

  aboutToAppear(): void {
    const lastError = this.rnAbility.getLastError()
    if (lastError) {
      this.handleError(lastError)
    }
    this.cleanUpListeners.push(this.rnAbility.eventEmitter.subscribe("NEW_ERROR", (err: RNOHError) => {
      this.handleError(err)
    }))
    createSafeAreaInsets(this.rnAbility.context, false).then((insets) => {
      this.insets = insets
    })
  }

  aboutToDisappear(): void {
    this.cleanUpListeners.forEach(cb => cb())
  }

  private handleError(err: RNOHError) {
    this.lastError = err
  }

  private getMessage(): string {
    return this.lastError?.getMessage() ?? ""
  }

  private getSuggestions(): string {
    const suggestions = this.lastError?.getSuggestions() ?? []
    if (suggestions.length === 1) {
      return suggestions[0]
    }
    return suggestions.map((s, idx) => `${idx + 1}) ${s}`).join("\n\n")
  }

  private getStack(): string {
    if (!this.lastError) {
      return ""
    }
    const stack = this.lastError.getStack()
    if (!stack) {
      return ""
    }
    return stack.split("\n").map(s => s.trim()).join("\n\n").trim()
  }

  private getDetails(): string {
    if (!this.lastError) {
      return ""
    }
    return this.lastError.getDetails() ?? ""
  }

  @Builder
  buildButton(label: string, onClick: () => void) {
    Button(label)
      .onClick(() => {
        onClick()
        this.controller.close()
        this.onClose()
      })
      .backgroundColor("#1A1A1A")
      .enabled(!(this.lastError instanceof FatalRNOHError))
  }

  build() {
    Column() {
      Flex({ direction: FlexDirection.Column }) {
        Scroll() {
          Column() {
            if (this.getMessage()) {
              Text(this.getMessage())
                .width("100%")
                .fontColor("white")
                .backgroundColor("#C13130")
                .padding({
                  top: GUTTER,
                  left: this.insets.left + GUTTER,
                  right: this.insets.right + GUTTER,
                  bottom: GUTTER
                })
                .fontWeight("bold")
            }
            if (this.getSuggestions()) {
              Text((this.lastError?.getSuggestions() ?? []).length === 1 ? "Suggestion" : "Suggestions")
                .width("100%")
                .fontColor("white")
                .fontSize(12)
                .fontWeight("bold")
                .backgroundColor("#6D2525")
                .padding({ top: GUTTER, left: this.insets.left + GUTTER, right: this.insets.right + GUTTER })
              Text(this.getSuggestions())
                .width("100%")
                .fontColor("white")
                .fontSize(12)
                .backgroundColor("#6D2525")
                .padding({
                  top: GUTTER / 2,
                  left: this.insets.left + GUTTER,
                  right: this.insets.right + GUTTER,
                  bottom: GUTTER
                })
            }
            if (this.getStack()) {
              Text("Stack")
                .width("100%")
                .fontColor("#888")
                .fontSize(12)
                .fontWeight("bold")
                .padding({ top: GUTTER, left: this.insets.left + GUTTER, right: this.insets.right + GUTTER })
              Text(this.getStack())
                .width("100%")
                .fontColor("#AAA")
                .fontSize(12)
                .padding({
                  top: GUTTER / 2,
                  left: this.insets.left + GUTTER,
                  right: this.insets.right + GUTTER,
                  bottom: GUTTER
                })
            }
            if (this.getDetails()) {
              Text("Details")
                .width("100%")
                .fontColor("#888")
                .fontSize(12)
                .fontWeight("bold")
                .padding({ top: GUTTER, left: this.insets.left + GUTTER, right: this.insets.right + GUTTER })
              Text(this.getDetails())
                .width("100%")
                .fontColor("#AAA")
                .fontSize(12)
                .padding({ top: GUTTER / 2, left: this.insets.left + GUTTER, right: this.insets.right + GUTTER })
            }
          }.padding({ bottom: GUTTER })
        }
        .width("100%")
        .flexGrow(1)
        .align(Alignment.TopStart)
        .edgeEffect(EdgeEffect.Spring)

        Row() {
          this.buildButton("Dismiss", () => {
          })
          this.buildButton("Reload", () => {
            this.rnAbility.devToolsController.reload()
          })
        }
        .width("100%")
        .height(64)
        .justifyContent(FlexAlign.SpaceEvenly)
        .borderWidth({ top: 1 })
        .borderColor("#666")
      }.backgroundColor("#1A1A1A")
    }
    .width("100%")
    .height("100%")
    .padding({ top: this.insets.top })
  }
}