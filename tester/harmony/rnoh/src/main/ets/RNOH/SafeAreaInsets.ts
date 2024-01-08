import type common from '@ohos.app.ability.common';
import WindowUtils from '@ohos.window';
import display from '@ohos.display';

export type SafeAreaInsets = {
  top: number,
  left: number,
  right: number,
  bottom: number
}

declare function px2vp(arg: number): number

export async function createSafeAreaInsets(uiAbilityContext: common.UIAbilityContext, isStatusBarHidden: boolean): Promise<SafeAreaInsets> {
  const win = await WindowUtils.getLastWindow(uiAbilityContext)
  const [displayCutoutInfo, systemAvoidArea, cutoutAvoidArea] = await Promise.all([
    display.getDefaultDisplaySync().getCutoutInfo(),
    win.getWindowAvoidArea(WindowUtils.AvoidAreaType.TYPE_SYSTEM),
    win.getWindowAvoidArea(WindowUtils.AvoidAreaType.TYPE_CUTOUT),
  ])
  const waterfallAvoidArea: WindowUtils.AvoidArea = {
    visible: true,
    leftRect: displayCutoutInfo.waterfallDisplayAreaRects.left,
    rightRect: displayCutoutInfo.waterfallDisplayAreaRects.right,
    topRect: displayCutoutInfo.waterfallDisplayAreaRects.top,
    bottomRect: displayCutoutInfo.waterfallDisplayAreaRects.bottom
  }
  const avoidAreas = [cutoutAvoidArea, waterfallAvoidArea]
  if (!isStatusBarHidden) {
    avoidAreas.push(systemAvoidArea)
  }
  const insets = getSafeAreaInsetsFromAvoidAreas(avoidAreas, win.getWindowProperties().windowRect)
  return mapProps(insets, (val) => px2vp(val))
}

function getSafeAreaInsetsFromAvoidAreas(avoidAreas: WindowUtils.AvoidArea[], windowSize: {
  width: number,
  height: number
}): SafeAreaInsets {
  return avoidAreas.reduce((currentInsets, avoidArea) => {
    return {
      top: Math.max(currentInsets.top, avoidArea.topRect.height + avoidArea.topRect.top),
      left: Math.max(currentInsets.left, avoidArea.leftRect.width + avoidArea.leftRect.left),
      right: Math.max(currentInsets.right, avoidArea.rightRect.left > 0 ? windowSize.width - avoidArea.rightRect.left : 0),
      bottom: Math.max(currentInsets.bottom, avoidArea.bottomRect.top > 0 ? windowSize.height - avoidArea.bottomRect.top : 0),
    }
  }, { top: 0, left: 0, right: 0, bottom: 0 })
}



function mapProps<TObj extends Record<string, any>>(obj: TObj, cb: <TKey extends keyof TObj>(value: TObj[TKey], key: TKey) => TObj[TKey]) {
  return Object.entries(obj).reduce((acc, [key, value]) => {
    acc[key as keyof TObj] = cb(value, key)
    return acc
  }, {} as Partial<TObj>) as TObj
}


