import React from 'react';
import GeneratedSampleNativeComponent, {
  OutgoingData,
  IncomingData,
} from './GeneratedSampleNativeComponent';
import { UIManager, findNodeHandle, processColor } from 'react-native';

export type GeneratedSampleComponentRef = {
  emitNativeEvent: (eventType: 'directEvent' | 'bubblingEvent') => void;
};

export const GeneratedSampleComponent = React.forwardRef<
  GeneratedSampleComponentRef,
  {
    children?: any;
    testProps: Omit<OutgoingData, 'colorTest'> & { colorTest: string };
    onDirectEvent?: (args: IncomingData) => void;
    onBubblingEvent?: (args: IncomingData) => void;
  }
>(
  (
    {
      children,
      testProps: { colorTest, ...otherTestProps },
      onDirectEvent,
      onBubblingEvent,
    },
    ref
  ) => {
    const nativeRef = React.useRef<any>(null);

    React.useImperativeHandle(
      ref,
      () => ({
        emitNativeEvent(eventType) {
          if (nativeRef?.current) {
            UIManager.dispatchViewManagerCommand(
              findNodeHandle(nativeRef.current),
              'emitNativeEvent',
              [eventType, false]
            );
          }
        },
      }),
      []
    );

    return (
      <GeneratedSampleNativeComponent
        ref={nativeRef}
        style={{
          width: '100%',
          height: 272,
          backgroundColor: 'white',
          borderWidth: 2,
          borderColor: 'pink',
        }}
        children={children}
        {...otherTestProps}
        colorTest={processColor(colorTest)!}
        onBubblingEvent={(e) => {
          onBubblingEvent?.(e.nativeEvent);
        }}
        onDirectEvent={(e) => {
          onDirectEvent?.(e.nativeEvent);
        }}
      />
    );
  }
);
