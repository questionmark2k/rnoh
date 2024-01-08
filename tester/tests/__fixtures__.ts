import type {TurboModule} from 'react-native/Libraries/TurboModule/RCTExport';
import {TurboModuleRegistry} from 'react-native';

interface SampleTurboModuleProtocol {
  voidFunc(): void;
  getBool(arg: boolean): boolean;
  getNull(arg: null): null;
  getString(arg: string): string;
  getObject(arg: {x: {y: number}}): Object;
  getArray(args: any[]): any[];
  registerFunction(onComplete: (value: string) => void): void;
  doAsyncJob(shouldResolve: boolean): Promise<string>;
  getPromisedArray(): Promise<number[]>;
  displayRNOHError(data: {
    whatHappened: string;
    howCanItBeFixed: string[];
    extraData?: any;
    customStack?: string;
  }): void;
  throwExceptionCpp(): never;
  throwExceptionArk(): never;
}

interface Spec extends TurboModule, SampleTurboModuleProtocol {}

export const SampleTurboModule =
  TurboModuleRegistry.get<Spec>('SampleTurboModule')!;
