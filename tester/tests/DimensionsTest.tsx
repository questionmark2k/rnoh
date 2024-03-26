import {TestSuite} from '@rnoh/testerino';
import React, {useState, useEffect} from 'react';
import {Text, StyleSheet, Dimensions} from 'react-native';
import {TestCase} from '../components';

export function DimensionsTest() {
  const [windowDimensions, setWindowDimensions] = useState(
    Dimensions.get('window'),
  );
  const [screenDimensions, setScreenDimensions] = useState(
    Dimensions.get('screen'),
  );

  useEffect(() => {
    const windowSubscription = Dimensions.addEventListener(
      'change',
      ({window}) => setWindowDimensions(window),
    );
    const screenSubscription = Dimensions.addEventListener(
      'change',
      ({screen}) => setScreenDimensions(screen),
    );
    return () => {
      windowSubscription.remove();
      screenSubscription.remove();
    };
  }, []);

  return (
    <TestSuite name="Dimensions">
      <TestCase.Logical
        itShould="export dimensions"
        fn={({expect}) => {
          expect(Dimensions).to.not.be.undefined;
          expect(Dimensions.get).to.not.be.undefined;
        }}
      />
      <TestCase.Logical
        itShould="gets window dimensions without throwing"
        fn={({expect}) => {
          expect(Dimensions.get.bind(Dimensions, 'window')).to.not.throw();
        }}
      />
      <TestCase.Logical
        itShould="gets screen dimensions without throwing"
        fn={({expect}) => {
          expect(Dimensions.get.bind(Dimensions, 'screen')).to.not.throw();
        }}
      />
      <TestCase.Example itShould="display window dimensions">
        <Text style={styles.text}>
          Window dimensions: {JSON.stringify(windowDimensions)}
        </Text>
      </TestCase.Example>
      <TestCase.Example itShould="display screen dimensions">
        <Text style={styles.text}>
          Screen dimensions: {JSON.stringify(screenDimensions)}
        </Text>
      </TestCase.Example>
    </TestSuite>
  );
}
const styles = StyleSheet.create({
  text: {
    fontSize: 14,
  },
});
