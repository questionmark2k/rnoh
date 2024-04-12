import {
  Animated,
  PanResponder,
  ScrollView,
  StyleSheet,
  View,
} from 'react-native';
import {TestSuite} from '@rnoh/testerino';
import {useRef} from 'react';
import {TestCase} from '../components';

export const PanResponderTest = () => {
  return (
    <TestSuite name="PanResponder">
      <TestCase.Logical
        tags={['C_API']}
        itShould="create PanResponder"
        fn={({expect}) => {
          expect(PanResponder.create({})).to.be.not.empty;
        }}
      />
      <TestCase.Example
        tags={['C_API']}
        itShould="allow panning inside ScrollView">
        <PanResponderInScrollView />
      </TestCase.Example>
    </TestSuite>
  );
};

const PanResponderInScrollView = () => {
  const pan = useRef(new Animated.ValueXY()).current;

  const panResponder = useRef(
    PanResponder.create({
      onMoveShouldSetPanResponder: () => true,
      onPanResponderMove: Animated.event(
        [null, {dx: pan.x, dy: pan.y}],
        undefined,
      ),
      onPanResponderRelease: () => {
        pan.extractOffset();
      },
      onShouldBlockNativeResponder: () => true,
    }),
  ).current;
  return (
    <ScrollView style={styles.scrollview} horizontal pagingEnabled>
      <View style={[styles.base, styles.view1]}>
        <Animated.View
          style={{
            transform: [{translateX: pan.x}, {translateY: pan.y}],
          }}
          {...panResponder.panHandlers}>
          <View style={styles.box} />
        </Animated.View>
      </View>
      <View style={[styles.base, styles.view2]} />
      <View style={[styles.base, styles.view1]} />
      <View style={[styles.base, styles.view2]} />
    </ScrollView>
  );
};

const styles = StyleSheet.create({
  view1: {
    backgroundColor: 'pink',
  },
  view2: {
    backgroundColor: 'powderblue',
  },
  base: {
    height: 400,
    width: 300,
  },
  scrollview: {
    borderWidth: 2,
    borderColor: 'black',
    height: 400,
    width: 300,
  },
  box: {
    height: 100,
    width: 100,
    backgroundColor: 'blue',
    borderRadius: 5,
  },
});
