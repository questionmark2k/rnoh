/**
 * This TurboModule was created to testing collisions in generated code.
 */

import { TurboModuleRegistry } from 'react-native';
import type { TurboModule } from 'react-native/Libraries/TurboModule/RCTExport';

export interface Spec extends TurboModule {
  voidFunc(): void;
}

export default TurboModuleRegistry.get<Spec>('GeneratedSampleTurboModule2')!;
