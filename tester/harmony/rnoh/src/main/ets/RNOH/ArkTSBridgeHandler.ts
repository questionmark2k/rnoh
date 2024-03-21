import { DisplayMetrics } from './types'

export interface ArkTSBridgeHandler {
  getDisplayMetrics: () => DisplayMetrics
}