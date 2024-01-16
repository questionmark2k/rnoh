import { TurboModuleRegistry } from 'react-native';
import type { TurboModule } from 'react-native/Libraries/TurboModule/RCTExport';

/**
 * Codegen restriction: All TypeScript interfaces extending TurboModule must be called 'Spec'.
 */
export interface Spec extends TurboModule {
  voidFunc(): void;
  getBool(arg: boolean): boolean;
  getString(arg: string): string;
  getObject(arg: { x: { y: number } }): Object;
  registerFunction(onComplete: (value: string) => void): void;
  doAsyncJob(shouldResolve: boolean): Promise<string>;
  getPromisedArray(): Promise<number[]>;
}

export default TurboModuleRegistry.get<Spec>('GeneratedSampleTurboModule')!;
