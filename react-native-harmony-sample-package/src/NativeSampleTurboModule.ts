import { TurboModuleRegistry } from 'react-native';
import { Spec } from './NativeGeneratedSampleTurboModule';

// Those types are not supported by codegen: null, any, never
interface SampleTurboModule extends Spec {
  getNull(arg: null): null;
  getArray(args: any[]): any[];
  displayRNOHError(data: {
    whatHappened: string;
    howCanItBeFixed: string[];
    extraData?: any;
    customStack?: string;
  }): void;
  throwExceptionCpp(): never;
  throwExceptionArk(): never;
}

export default TurboModuleRegistry.get<SampleTurboModule>('SampleTurboModule')!;
