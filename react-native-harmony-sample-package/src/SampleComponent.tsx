import React, { useRef } from 'react';
import { SampleNativeComponent } from './SampleNativeComponent';
import { StyleProp, UIManager, ViewStyle, findNodeHandle } from 'react-native';

export type SampleComponentRef = { toggleFontSize: () => void };

export const SampleComponent = React.forwardRef<
  SampleComponentRef,
  {
    children?: any;
    backgroundColor: string;
    size: number;
  }
>(({ children, backgroundColor, size }, ref) => {
  const nativeRef = useRef<any>(null);

  React.useImperativeHandle(
    ref,
    () => ({
      toggleFontSize() {
        if (nativeRef?.current) {
          UIManager.dispatchViewManagerCommand(
            findNodeHandle(nativeRef.current),
            'toggleFontSize',
            []
          );
        }
      },
    }),
    []
  );

  const style: StyleProp<ViewStyle> = {
    backgroundColor: backgroundColor,
    width: size,
    height: size,
    display: 'flex',
    justifyContent: 'center',
    alignItems: 'center',
    borderColor: 'green',
    borderWidth: 1,
    borderRadius: 4,
    opacity: 1,
  };

  return (
    <SampleNativeComponent
      ref={nativeRef}
      style={style}
      size={size}
      children={children}
    />
  );
});
