import { TurboModule, FatalRNOHError, RNOHError } from "../../RNOH/ts";

export type StackFrame = {
  column?: number,
  file?: string,
  lineNumber?: number,
  methodName: string,
  collapse?: boolean,
};

export type ExceptionData = {
  message: string,
  originalMessage?: string,
  name?: string,
  componentStack?: string,
  stack: StackFrame[],
  id: number,
  isFatal: boolean,
  extraData?: Object,
};

/**
 * Note: This TurboModule is used when __DEV__ flag in JS bundle is set to false.
 */
export class ExceptionsManagerTurboModule extends TurboModule {
  public static readonly NAME = 'ExceptionsManager';

  reportFatalException(message: string, stack: StackFrame[], exceptionId: number): void {
    this.ctx.logger.error(new FatalRNOHError({
      whatHappened: message,
      howCanItBeFixed: [],
      customStack: stringifyStack(stack),
    }))
  }

  reportSoftException(message: string, stack: StackFrame[], exceptionId: number): void {
    this.ctx.logger.error(new RNOHError({
      whatHappened: message,
      howCanItBeFixed: [],
      customStack: stringifyStack(stack)
    }))
  }

  reportException(data: ExceptionData): void {
    if (data.isFatal) {
      this.ctx.logger.error(new FatalRNOHError({
        whatHappened: data.message,
        howCanItBeFixed: [],
        customStack: stringifyStack(data.stack),
        extraData: data.extraData
      }))
    } else {
      this.ctx.logger.error(new RNOHError({
        whatHappened: data.originalMessage,
        howCanItBeFixed: [],
        customStack: stringifyStack(data.stack),
        extraData: data.extraData
      }))
    }
  }


  updateExceptionMessage(message: string, stack: StackFrame[], exceptionId: number): void {
    this.ctx.logger.warn(`ExceptionsManager::updateExceptionMessage is not implemented`);
    this.ctx.logger.error(`ExceptionsManager::updateExceptionMessage ${message}`);
    stack.forEach((frame) => {
      this.ctx.logger.error(JSON.stringify(frame));
    });
  }

  dismissRedbox(): void {
    this.ctx.devToolsController.dismissRNOHErrorDialog()
  }
}

function stringifyStack(stackFrames: StackFrame[]): string {
  return stackFrames.map(entry => {
    let location = ""
    let locationInFile = ""
    if (entry.lineNumber) {
      if (entry.column !== undefined) {
        locationInFile = `${entry.lineNumber}:${entry.column}`
      } else {
        locationInFile = `${entry.lineNumber}`
      }
    }
    if (entry.file) {
      if (locationInFile) {
        location = `${entry.file}:${locationInFile}`
      } else {
        location = `${entry.file}`
      }
    } else {
      location = locationInFile
    }
    if (location) {
      return `${entry.methodName} (${location})`
    } else {
      return `${entry.methodName}`
    }
  }).join("\n")
}