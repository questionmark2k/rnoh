import { RNAbility, ErrorEventEmitter, Edges, RNOHError } from "./RNOH"
import { SafeAreaTurboModule } from "./RNOHCorePackage/turboModules"

@Component
export struct RNOHErrorDialog {
  public controller!: CustomDialogController
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
      alignment: DialogAlignment.TopStart,
      backgroundColor: Color.Transparent,
      builder: RNOHErrorCustomDialog({
        errorEventEmitter: this.rnAbility!.errorEventEmitter,
        rnAbility: this.rnAbility!
      })
    })
    this.cleanUpListeners.push(this.rnAbility!.errorEventEmitter.subscribe("NEW_ERROR", (err) => {
        this.controller.open()
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
  public errorEventEmitter!: ErrorEventEmitter
  public rnAbility!: RNAbility
  private cleanUpListeners: (() => void)[] = []
  @State private insets: Edges<number> = { top: 0, left: 0, right: 0, bottom: 0 }
  @State private lastError: RNOHError | null = null

  aboutToAppear(): void {
    const lastError = this.rnAbility.getLastError()
    if (lastError) {
      this.handleError(lastError)
    }
    this.cleanUpListeners.push(this.errorEventEmitter.subscribe("NEW_ERROR", (err) => {
      this.handleError(err)
    }))
    SafeAreaTurboModule.createInsets(this.rnAbility.context, false).then((insets) => {
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
    return suggestions.map((s, idx) => `${idx + 1}) ${s}`).join("\n\n")
  }

  private getStack(): string {
    if (!this.lastError) {
      return ""
    }
    const stack = this.lastError.stack
    if (!stack) {
      return ""
    }
    return stack.split("\n").map(s => s.trim()).join("\n\n").trim()
  }

  private getDetails() {
    if (!this.lastError) {
      return ""
    }
    const details = this.lastError.getDetails() ?? ""
    return details.split("\n").map(s => s.trim()).join("\n\n").trim()
  }

  @Builder
  buildButton(label: string, onClick: () => void) {
    Button(label)
      .onClick(onClick)
      .backgroundColor("#1A1A1A")
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
              Text(this.getSuggestions())
                .width("100%")
                .fontColor("white")
                .fontSize(12)
                .backgroundColor("#6D2525")
                .padding({
                  top: GUTTER,
                  left: this.insets.left + GUTTER,
                  right: this.insets.right + GUTTER,
                  bottom: GUTTER
                })
            }
            if (this.getStack()) {
              Text(this.getStack())
                .width("100%")
                .fontColor("#AAA")
                .fontSize(12)
                .padding({
                  top: GUTTER,
                  left: this.insets.left + GUTTER,
                  right: this.insets.right + GUTTER,
                  bottom: GUTTER
                })
            }
            if (this.getDetails()) {
              Text(this.getDetails())
                .width("100%")
                .fontColor("#AAA")
                .fontSize(12)
                .padding({ top: GUTTER, left: this.insets.left + GUTTER, right: this.insets.right + GUTTER })
            }
          }
        }
        .width("100%")
        .flexGrow(1)
        .align(Alignment.TopStart)

        Row() {
          this.buildButton("Dismiss", () => {
            this.controller.close()
          })
          this.buildButton("Reload", () => {
            this.rnAbility.devToolsController.reload()
            this.controller.close()
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