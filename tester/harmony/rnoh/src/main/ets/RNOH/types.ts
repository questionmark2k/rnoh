export type Point = {
  x: number,
  y: number,
}

export type Position = {
  x: number,
  y: number,
}

export type BoundingBox = {
  left: number,
  right: number,
  top: number,
  bottom: number,
}

export interface Size {
  width: number;
  height: number;
}

export type Edges<T> = {
  top: T,
  left: T,
  right: T,
  bottom: T
}

export type Corners<T> = {
  topLeft: T,
  topRight: T,
  bottomLeft: T,
  bottomRight: T,
}

export type PointerEvents = "auto" | "none" | "box-none" | "box-only"

export type DisplayMetrics = {
  windowPhysicalPixels: PhysicalPixels,
  screenPhysicalPixels: PhysicalPixels,
};

export type PhysicalPixels = {
  width: number,
  height: number,
  scale: number,
  fontScale: number,
  densityDpi: number,
}

export type InspectorPageId = number

export interface InspectorPage {
  id: InspectorPageId,
  title: string,
  vm: string
}

// RemoteConnection allows the VM to send debugger messages to the debugger.
export interface InspectorRemoteConnection {
  onMessage(message: string)
  onDisconnect()
}

// LocalConnection allows the debugger to send debugger messages to the VM.
export interface InspectorLocalConnection {
  sendMessage(payload: unknown)
  disconnect()
}

export interface InspectorInstance {
  getPages(): InspectorPage[]
  connect(pageId: InspectorPageId, remote: InspectorRemoteConnection): InspectorLocalConnection
}
