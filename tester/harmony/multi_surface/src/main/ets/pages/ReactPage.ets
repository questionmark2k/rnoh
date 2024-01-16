import type { ComponentBuilderContext} from 'rnoh';
import { RNAbility, RNInstance, RNSurface, SurfaceHandle} from 'rnoh'
import { SAMPLE_VIEW_TYPE, SampleView, PropsDisplayer } from 'rnoh-sample-package'
import router from '@ohos.router'
import { SurfaceConfig1 } from 'rnoh/src/main/ets/RNSurface';

interface PageParams {
  appKey: string
}

@Entry
@Component
struct ReactPage {
  @State appKey: string = (router.getParams() as PageParams).appKey
  @State surfaceHandle: SurfaceHandle | undefined = undefined;
  @State showSurface: boolean = false;
  @StorageLink('RNAbility') private rnAbility: RNAbility | undefined = undefined
  @StorageLink('AppRNInstance') rnInstance: RNInstance | undefined = undefined

  @Builder
  buildCustomComponent(ctx: ComponentBuilderContext) {
    if (ctx.descriptor.type === SAMPLE_VIEW_TYPE) {
      SampleView({
        ctx: ctx.rnohContext,
        tag: ctx.descriptor.tag,
        buildCustomComponent: this.buildCustomComponent
      })
    } else if (ctx.descriptor.type === PropsDisplayer.NAME) {
      PropsDisplayer({
        ctx: ctx.rnohContext,
        tag: ctx.descriptor.tag
      })
    }
  }

  onBackPress(): boolean {
    return this.rnAbility?.onBackPress() ?? false;
  }

  getOrCreateSurface() {
    const surfacesMap: Record<string, SurfaceHandle> = AppStorage.get("ReactSurfaceByAppKey") ?? {};

    if (surfacesMap[this.appKey]) {
      return surfacesMap[this.appKey];
    }

    const surfaceHandle = this.rnInstance?.createSurface(this.appKey);
    if (surfaceHandle !== undefined) {
      surfaceHandle.start(
        {
          width: 384,
          height: 715,
          surfaceOffsetX: 0,
          surfaceOffsetY: 0,
          pixelRatio: vp2px(1)
        },
        {});
      surfacesMap[this.appKey] = surfaceHandle;
    }
    return surfaceHandle;
  }

  aboutToAppear() {
    this.surfaceHandle = this.getOrCreateSurface();
  }

  aboutToDisappear() {}

  build() {
    Column() {
      Row() {
        Navigator({ target: "pages/Index", type: NavigationType.Back }) {
          Text("Go back")
            .width('100%')
            .margin({ left: 12 })
            .fontColor(Color.Blue)
        }
      }.height("5%")


      if (!this.showSurface) {
        Row() {
          Button("Show Surface")
            .onClick(() => this.showSurface = !this.showSurface)
        }
        .height("5%")
        .enabled(this.surfaceHandle !== undefined)
      } else if (this.rnAbility && this.rnInstance && this.surfaceHandle) {
        Row() {
          RNSurface({
            ctx: this.rnAbility.createRNOHContext({ rnInstance: this.rnInstance }),
            buildCustomComponent: this.buildCustomComponent,
            surfaceConfig: { surfaceHandle: this.surfaceHandle } as SurfaceConfig1,
          })
        }
        .height("95%")
      }
    }
    .width("100%")
    .height("100%")
    .margin(32)
  }
}