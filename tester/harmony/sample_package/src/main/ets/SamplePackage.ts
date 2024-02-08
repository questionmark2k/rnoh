import { RNPackage, TurboModulesFactory } from 'rnoh/ts';
import type {
  TurboModule,
  TurboModuleContext,
  DescriptorWrapperFactoryByDescriptorTypeCtx,
  DescriptorWrapperFactoryByDescriptorType
} from 'rnoh/ts';
import { TM, RNC } from "rnoh/generated/ts"
import { SampleTurboModule } from './SampleTurboModule';

class SampleTurboModulesFactory extends TurboModulesFactory {
  createTurboModule(name: string): TurboModule | null {
    if (name === 'SampleTurboModule' || name === TM.GeneratedSampleTurboModule.NAME || name === TM.GeneratedSampleTurboModule2.NAME) {
      return new SampleTurboModule(this.ctx);
    }
    return null;
  }

  hasTurboModule(name: string): boolean {
    return name === 'SampleTurboModule' || name === TM.GeneratedSampleTurboModule.NAME || name === TM.GeneratedSampleTurboModule2.NAME;
  }
}

export class SamplePackage extends RNPackage {
  createTurboModulesFactory(ctx: TurboModuleContext): TurboModulesFactory {
    return new SampleTurboModulesFactory(ctx);
  }

  createDescriptorWrapperFactoryByDescriptorType(ctx: DescriptorWrapperFactoryByDescriptorTypeCtx): DescriptorWrapperFactoryByDescriptorType {
    return {
      [RNC.GeneratedSampleView.NAME]: (ctx) => new RNC.GeneratedSampleView.DescriptorWrapper(ctx.descriptor)
    }
  }
}
