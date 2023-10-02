import UIAbility from '@ohos.app.ability.UIAbility'
import common from '@ohos.app.ability.common'
import type { Tag } from './DescriptorBase'
import type { CommandDispatcher } from "./CommandDispatcher"
import type { DescriptorRegistry } from "./DescriptorRegistry"
import type { RNPackage, RNPackageContext } from "./RNPackage"
import type { TurboModule } from "./TurboModule"
import type { JSBundleProvider } from "./JSBundleProvider"
import { ComponentManagerRegistry } from './ComponentManagerRegistry'
import { SurfaceHandle } from './SurfaceHandle'

export type SurfaceContext = {
  width: number
  height: number
  surfaceOffsetX: number
  surfaceOffsetY: number
}

export enum LifecycleState {
  BEFORE_CREATE,
  PAUSED,
  READY,
}

export type LifecycleEventArgsByEventName = {
  CONFIGURATION_UPDATE: Parameters<UIAbility["onConfigurationUpdate"]>
  FOREGROUND: [];
  BACKGROUND: [];
  JS_BUNDLE_EXECUTION_FINISH: [{ jsBundleUrl: string, appKeys: string[] }];
}

export type BundleExecutionStatus = "RUNNING" | "DONE"

export interface RNInstance {
  descriptorRegistry: DescriptorRegistry;
  commandDispatcher: CommandDispatcher;
  componentManagerRegistry: ComponentManagerRegistry;
  abilityContext: common.UIAbilityContext;

  getLifecycleState(): LifecycleState;

  subscribeToLifecycleEvents: <TEventName extends keyof LifecycleEventArgsByEventName>(
    eventName: TEventName,
    listener: (...args: LifecycleEventArgsByEventName[TEventName]) => void
  ) => () => void;

  callRNFunction(moduleName: string, functionName: string, args: unknown[]): void;

  emitDeviceEvent(eventName: string, payload: any): void;

  emitComponentEvent(tag: Tag, eventEmitRequestHandlerName: string, payload: any): void;

  getBundleExecutionStatus(bundleURL: string): BundleExecutionStatus | undefined

  runJSBundle(jsBundleProvider: JSBundleProvider): Promise<void>;

  getTurboModule<T extends TurboModule>(name: string): T;

  createSurface(appKey: string): SurfaceHandle;

  updateState(componentName: string, tag: Tag, state: unknown): void;

  initialize(): void;

  getId(): number;
}

export type RNInstanceOptions = {
  createRNPackages: (ctx: RNPackageContext) => RNPackage[]
}