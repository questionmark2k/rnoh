import { DisplayMetrics } from './types'

export interface ArkTSBridgeHandler {
  getDisplayMetrics: () => DisplayMetrics
  handleError: (errData: {message: string, stacktrace?: string[], suggestions?: string[]}) => void
}