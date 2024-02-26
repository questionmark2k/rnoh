import {TestCase, TestSuite} from '@rnoh/testerino';
import {useEffect, useState} from 'react';
import {AccessibilityInfo, Text, View} from 'react-native';

export function AccessibilityInfoTest() {
  return (
    <TestSuite name="AccessibilityInfo">
      <TestCase
        skip
        itShould="display red background if Screen Reader is enabled">
        <AccessibilityInfoScreenReaderStatus />
      </TestCase>
    </TestSuite>
  );
}

function AccessibilityInfoScreenReaderStatus() {
  const [isEnabled, setIsEnabled] = useState(false);
  const backgroundColor = isEnabled ? 'red' : 'transparent';

  useEffect(() => {
    const listener = AccessibilityInfo.addEventListener(
      'screenReaderChanged',
      setIsEnabled,
    );

    AccessibilityInfo.isScreenReaderEnabled()
      .then(isOptionEnabled => {
        setIsEnabled(isOptionEnabled);
      })
      .catch(err =>
        console.log(`Error while testing Screen Reader - error: ${err}`),
      );

    return function cleanup() {
      listener.remove();
    };
  }, []);

  return (
    <View style={{backgroundColor, padding: 16}}>
      <Text>{`Screen Reader is ${isEnabled ? 'enabled' : 'disabled'}.`}</Text>
    </View>
  );
}
