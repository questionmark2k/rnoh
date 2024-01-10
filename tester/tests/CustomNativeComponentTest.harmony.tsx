import {TestCase, TestSuite} from '@rnoh/testerino';
import {
  SampleComponent,
  SampleComponentRef,
} from 'react-native-harmony-sample-package';
import {useState} from 'react';
import React from 'react';
import {Button, Ref} from '../components';

export function CustomNativeComponentTest() {
  return (
    <TestSuite name="Custom Native Component">
      <TestCase itShould="render red rectangle">
        <SampleComponent backgroundColor="red" size={64} />
      </TestCase>
      <TestCase itShould="render green rectangle inside red rectangle">
        <SampleComponent backgroundColor="red" size={64}>
          <SampleComponent backgroundColor="green" size={32} />
        </SampleComponent>
      </TestCase>
      <TestCase itShould="show/hide blue rectangle">
        <SampleComponent backgroundColor="red" size={64}>
          <Blinker>
            <SampleComponent backgroundColor="blue" size={32} />
          </Blinker>
        </SampleComponent>
      </TestCase>
      <TestCase itShould="toggle font size in the component below button (native commands)">
        <Ref<SampleComponentRef>
          render={ref => {
            return (
              <>
                <Button
                  label="Toggle Font Size"
                  onPress={() => {
                    ref.current?.toggleFontSize();
                  }}
                />
                <SampleComponent ref={ref} backgroundColor="blue" size={32} />;
              </>
            );
          }}
        />
      </TestCase>
    </TestSuite>
  );
}

function Blinker({children}: any) {
  const [isVisible, setIsVisible] = useState(true);

  return (
    <React.Fragment>
      <Button
        label="Run"
        onPress={() => {
          setIsVisible(prev => !prev);
        }}
      />
      {isVisible && children}
    </React.Fragment>
  );
}
