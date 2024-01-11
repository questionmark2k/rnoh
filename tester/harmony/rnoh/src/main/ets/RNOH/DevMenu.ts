import window from '@ohos.window';
import { DevToolsController } from './DevToolsController';
import { RNOHLogger } from './RNOHLogger';
import type common from '@ohos.app.ability.common';


interface AlertDialogButtonOptions {
  value: string,
  action: () => void,
}

enum DialogButtonDirection {
  AUTO, HORIZONTAL, VERTICAL,
}


export class DevMenu {
  private devMenuDialogVisible: boolean = false;
  private devMenuButtons: AlertDialogButtonOptions[] = []
  private logger: RNOHLogger

  constructor(private devToolsController: DevToolsController, private uiAbilityContext: common.UIAbilityContext, logger: RNOHLogger) {
    this.logger = logger.clone("DevMenu")
    this.createDevMenuDefaultButtons()
  }

  private createDevMenuDefaultButtons() {
    this.devMenuButtons.push({
      value: "Reload",
      action: () => {
        this.devToolsController.reload(undefined)
        this.devMenuDialogVisible = false;
      },
    });
    this.devMenuButtons.push({
      value: "Toggle Element Inspector",
      action: () => {
        this.devToolsController.toggleElementInspector()
        this.devMenuDialogVisible = false;
      },
    });
  }

  public show() {
    if (!this.devMenuDialogVisible) {
      this.showDevMenuDialog();
    }
  }

  private showDevMenuDialog() {
    window.getLastWindow(this.uiAbilityContext).then((value) => {
      {
        const uiContext = value.getUIContext()
        const dialogParams = {
          title: "React Native Dev Menu",
          message: "",
          buttons: this.devMenuButtons,
          buttonDirection: DialogButtonDirection.VERTICAL,
          cancel: () => {
            this.devMenuDialogVisible = false;
          },
        }
        uiContext.showAlertDialog(dialogParams)
        this.devMenuDialogVisible = true;
        this.devToolsController.emitDevMenuShown();
      }
    }).catch(() => {
      this.logger.error("DevMenu dialog couldn't be displayed.");
      this.devMenuDialogVisible = false;
    })
  }
}