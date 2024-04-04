import { TurboModule, RNOHError, Tag } from 'rnoh/ts';
import { TM } from "rnoh/generated/ts"


export class SampleTurboModule extends TurboModule implements TM.GeneratedSampleTurboModule.Spec, TM.GeneratedSampleTurboModule2.Spec {
  getUnionValue(arg: null | Object): Object {
    return arg ?? {}
  }

  getRootTag(arg: Tag): Tag {
    return arg
  }

  getEnum(enum1: TM.GeneratedSampleTurboModule.SomeEnum1, enum2: TM.GeneratedSampleTurboModule.SomeEnum2, enum3: TM.GeneratedSampleTurboModule.SomeEnum3): {
    enum1: TM.GeneratedSampleTurboModule.SomeEnum1;
    enum2: TM.GeneratedSampleTurboModule.SomeEnum2;
    enum3: TM.GeneratedSampleTurboModule.SomeEnum3;
    hardcodedEnum1: TM.GeneratedSampleTurboModule.SomeEnum1;
  } {
    return { enum1, enum2, enum3, hardcodedEnum1: TM.GeneratedSampleTurboModule.SomeEnum1.FOO }
  }

  voidFunc() {
    console.log('RNOH SampleTurboModule::voidFunc');
  }

  getBool(arg: boolean): boolean {
    console.log(`RNOH SampleTurboModule::getBool(${arg})`);
    return arg;
  }

  getNull(arg: null) {
    console.log(`RNOH SampleTurboModule::getNull(${arg})`);
    return arg;
  }

  getString(arg: string): string {
    console.log(`RNOH SampleTurboModule::getString(${arg})`);
    return arg;
  }

  getObject(arg: { x: { y: number } }): Object {
    console.log(`RNOH SampleTurboModule::getObject(${arg})`);
    return arg;
  }

  getArray(args: any[]): any[] {
    console.log(`RNOH SampleTurboModule::getArray(${args})`);
    return args;
  }

  registerFunction(onComplete: (value: string) => void): void {
    console.log(`RNOH SampleTurboModule::registerFunction + trigger`);
    setTimeout(() => {
      onComplete?.('... from native side');
    }, 1000);
  }

  doAsyncJob(shouldResolve: boolean): Promise<string> {
    return new Promise((resolve, reject) => {
      setTimeout(() => {
        if (shouldResolve) {
          resolve('resolved on native side');
        } else {
          reject('rejected on native side');
        }
      }, 1000);
    });
  }

  getPromisedArray() {
    return Promise.resolve([1, 2, 3])
  }

  displayRNOHError(data: {
    whatHappened: string,
    howCanItBeFixed: string[]
    extraData?: any
    customStack?: string
  }) {
    this.ctx.logger.error(new RNOHError(data))
  }

  throwExceptionArk() {
    throw new Error("Exception thrown from ArkTS")
  }

  getUnknown(arg: Object) {
    return arg
  }

  async emitEventFromArkTS2Cpp(payload: { foo: string }): Promise<{ foo: string }> {
    return await new Promise((resolve) => {
      const unsubscribe = this.ctx.rnInstance.cppEventEmitter.subscribe("SAMPLE_MESSAGE", (value: { foo: string }) => {
        resolve(value)
        unsubscribe();
      })
      this.ctx.rnInstance.postMessageToCpp("SAMPLE_MESSAGE", payload);
    })
  }

  setNativeResponderBlocked(isBlocked: boolean, origin: string, componentInstanceId: string): void {
    this.ctx.rnInstance.postMessageToCpp("BLOCK_NATIVE_RESPONDER", { isBlocked, origin, componentInstanceId })
  }
}
