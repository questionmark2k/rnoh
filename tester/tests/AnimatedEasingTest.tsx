import React, {useRef} from 'react';
import {Animated, View, Easing, EasingFunction} from 'react-native';
import {TestSuite} from '@rnoh/testerino';
import {TestCase} from '../components';

export function AnimatedEasingTest() {
  return (
    <TestSuite name="Easing">
      <TestCase.Example itShould="move square linearly">
        <EasingView easing={Easing.linear} />
      </TestCase.Example>
      <TestCase.Example itShould="move square slowly accelerating">
        <EasingView easing={Easing.ease} />
      </TestCase.Example>
      <TestCase.Example itShould="move square with backwards cubic function">
        <EasingView easing={Easing.out(Easing.cubic)} />
      </TestCase.Example>
    </TestSuite>
  );
}

const EasingView = (props: {easing: EasingFunction}) => {
  const translateValue = useRef(new Animated.Value(0)).current;
  const animation = Animated.timing(translateValue, {
    toValue: 200,
    duration: 2000,
    easing: props.easing,
    useNativeDriver: true,
  });
  const handleAnimation = () => {
    animation.reset();
    animation.start();
  };
  return (
    <View style={{height: 60, width: '100%'}} onTouchEnd={handleAnimation}>
      <Animated.View
        style={{
          height: 40,
          width: 40,
          margin: 10,
          backgroundColor: 'red',
          transform: [
            {
              translateX: translateValue,
            },
          ],
        }}
      />
    </View>
  );
};
