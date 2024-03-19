import {TestSuite} from '@rnoh/testerino';
import {
  SampleComponent,
  SampleComponentRef,
  GeneratedSampleComponent,
  GeneratedSampleComponentRef,
} from 'react-native-sample-package';
import {useEffect, useState} from 'react';
import React from 'react';
import {Button, Effect, Ref, TestCase} from '../components';
import {IncomingData} from 'react-native-harmony-sample-package/src/GeneratedSampleNativeComponent';
import {View} from 'react-native';

export function CustomNativeComponentTest() {
  return (
    <TestSuite name="Custom Native Component">
      <ManualCustomComponentImplementationTest />
      <GeneratedCustomComponentTest />
    </TestSuite>
  );
}

function ManualCustomComponentImplementationTest() {
  const [refreshKey, setRefreshKey] = useState(0);

  useEffect(() => {
    const intervalId = setInterval(() => {
      setRefreshKey(prev => prev + 1);
    }, 1000);
    return () => {
      clearInterval(intervalId);
    };
  }, []);

  return (
    <TestSuite name="no codegen">
      <TestCase.Example tags={['C_API']} itShould="render red rectangle">
        <SampleComponent backgroundColor="red" size={64} />
      </TestCase.Example>
      <TestCase.Example itShould="render green rectangle inside red rectangle">
        <SampleComponent backgroundColor="red" size={64}>
          <SampleComponent backgroundColor="green" size={32} />
        </SampleComponent>
      </TestCase.Example>
      <TestCase.Example tags={['C_API']} itShould="change bgColor every second">
        <SampleComponent
          backgroundColor={refreshKey % 2 === 0 ? 'red' : 'green'}
          size={64}
        />
      </TestCase.Example>
      <TestCase.Example tags={['C_API']} itShould="show and hide red rectangle">
        <View style={{height: 64}}>
          {refreshKey % 2 === 0 && (
            <SampleComponent
              backgroundColor={refreshKey % 2 === 0 ? 'red' : 'green'}
              size={64}
            />
          )}
        </View>
      </TestCase.Example>
      <TestCase.Example itShould="show/hide blue rectangle">
        <SampleComponent backgroundColor="red" size={64}>
          <Blinker>
            <SampleComponent backgroundColor="blue" size={32} />
          </Blinker>
        </SampleComponent>
      </TestCase.Example>
      <TestCase.Example itShould="toggle font size in the component below button (native commands)">
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
                <SampleComponent ref={ref} backgroundColor="blue" size={32} />
              </>
            );
          }}
        />
      </TestCase.Example>
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

function GeneratedCustomComponentTest() {
  return (
    <TestSuite name="generated custom component">
      <TestCase.Manual<IncomingData | undefined>
        itShould="ensure equality between provided and received data"
        initialState={undefined}
        arrange={({setState}) => {
          return (
            <Ref<GeneratedSampleComponentRef>
              render={ref => (
                <GeneratedSampleComponent
                  ref={ref}
                  testProps={{
                    booleanTest: true,
                    intTest: 42,
                    floatTest: 42.5,
                    doubleTest: 42.5,
                    stringTest: 'foobar',
                    objectTest: {foo: {bar: 'baz'}},
                    colorTest: 'red',
                    arrayTest: ['foo', 'bar'],
                    readOnlyArrayTest: ['foo', 'bar'],
                    intEnumTest: 1,
                  }}
                  onDirectEvent={setState}>
                  <Effect
                    onMount={() => {
                      ref.current?.emitNativeEvent('directEvent');
                    }}
                  />
                </GeneratedSampleComponent>
              )}
            />
          );
        }}
        assert={({expect, state}) => {
          expect(state?.booleanTest).to.be.true;
          expect(state?.booleanWithDefaultTest).to.be.true;
          expect(state?.intTest).to.be.eq(42);
          expect(state?.intWithDefault).to.be.eq(42);
          expect(state?.floatTest).closeTo(42.5, 0.001);
          expect(state?.floatWithDefaultTest).closeTo(42.5, 0.001);
          expect(state?.doubleTest).closeTo(42.5, 0.001);
          expect(state?.doubleWithDefaultTest).closeTo(42.5, 0.001);
          expect(state?.stringTest).to.be.eq('foobar');
          expect(state?.stringWithDefaultTest).to.be.eq('foobar');
          expect(state?.objectTest).to.deep.eq({foo: {bar: 'baz'}});
          expect(state?.arrayTest).to.deep.eq(['foo', 'bar']);
          expect(state?.readOnlyArrayTest).to.deep.eq(['foo', 'bar']);
          expect(state?.stringEnumTest).to.be.eq('foo');
          expect(state?.intEnumTest).to.be.eq(1);
        }}
      />
    </TestSuite>
  );
}
