/**
 * This TurboModule was created to test collisions in the generated code.
 */

import { TurboModuleRegistry } from 'react-native';
import type { TurboModule } from 'react-native/Libraries/TurboModule/RCTExport';

export interface Spec extends TurboModule {
  voidFunc(): void;
  emitEventFromArkTS2Cpp(payload: { foo: string }): Promise<{ foo: string }>;
  setNativeResponderBlocked(
    isBlocked: boolean,
    origin: string,
    componentId: string
  ): void;
}

export default TurboModuleRegistry.get<Spec>('GeneratedSampleTurboModule2')!;
