import { TurboModule, RNOHError } from 'rnoh/ts';

export class SampleTurboModule extends TurboModule {
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
    console.log(`RNOH SampleTurboModule::getString(${arg})`);
    return arg;
  }

  getArray(args: any[]): any[] {
    console.log(`RNOH SampleTurboModule::getArray(${args})`);
    return args;
  }

  getNumber(args: any[]): any[] {
    console.log(`RNOH SampleTurboModule::getNumber(${args})`);
    return args;
  }

  getValue(x: number, y: string, z: object): object {
    console.log(`RNOH SampleTurboModule::getValue(${x},${y},${z})`);
    return {x, y, z};
  }

  getValueWithPromise(error: boolean): Promise<string> {
    return new Promise((resolve, reject) => {
      if(error) {
        reject('intentional');
      } else {
        resolve('result!')
      }
    })
  }

  getValueWithCallback(callback: (value: string) => void): void {
    callback('value from callback')
  }

  getConstants(): {const1: boolean, const2: number, const3: string} {
    console.log(`RNOH SampleTurboModule::getConstants`);
    return {const1: true, const2: 1228, const3: "something"};
  }

  getRootTag(args: number): number {
    console.log(`RNOH SampleTurboModule::getRootTag(${args})`);
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
    return Promise.resolve([1,2,3])
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
}
