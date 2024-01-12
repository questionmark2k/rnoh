import {TestCase, TestSuite} from '@rnoh/testerino';
import {
  COMMON_PROPS,
  getScrollViewContent,
  getScrollViewContentHorizontal,
} from './fixtures';
import {ScrollView, StyleSheet, View} from 'react-native';
import React from 'react';
import {Button} from '../../components';

export function ScrollBarsTest() {
  return (
    <TestSuite name="scroll indicators / scrollbar">
      <TestCase modal itShould="have persistent scrollbar">
        <View style={styles.wrapperView}>
          <ScrollView persistentScrollbar={true} {...COMMON_PROPS} />
        </View>
      </TestCase>
      <TestCase modal itShould="shows white vertical scroll indicator">
        <View style={styles.wrapperView}>
          <ScrollView {...COMMON_PROPS} indicatorStyle={'white'} />
        </View>
      </TestCase>
      <TestCase modal itShould="show vertical scroll indicator">
        <View style={styles.wrapperView}>
          <ScrollView {...COMMON_PROPS} showsVerticalScrollIndicator={true} />
        </View>
      </TestCase>
      <TestCase modal itShould="hide vertical scroll indicator">
        <View style={styles.wrapperView}>
          <ScrollView showsVerticalScrollIndicator={false} {...COMMON_PROPS} />
        </View>
      </TestCase>
      <TestCase modal itShould="show horizontal scroll indicator">
        <View style={styles.wrapperView}>
          <ScrollView
            showsHorizontalScrollIndicator={true}
            horizontal
            {...COMMON_PROPS}>
            {getScrollViewContentHorizontal({})}
          </ScrollView>
        </View>
      </TestCase>
      <TestCase modal itShould="hide horizontal scroll indicator">
        <View style={styles.wrapperView}>
          <ScrollView
            showsHorizontalScrollIndicator={false}
            horizontal
            {...COMMON_PROPS}>
            {getScrollViewContentHorizontal({})}
          </ScrollView>
        </View>
      </TestCase>
      <TestCase modal itShould="flash scroll indicators">
        <FlashIndicatorsTest />
      </TestCase>
    </TestSuite>
  );
}

function FlashIndicatorsTest() {
  const scrollRef = React.useRef<ScrollView>(null);
  return (
    <View style={styles.wrapperView}>
      <Button
        label={'Flash Indicators'}
        onPress={() => {
          scrollRef.current?.flashScrollIndicators();
        }}
      />
      <ScrollView
        style={{flex: 1}}
        scrollEnabled={true}
        showsVerticalScrollIndicator={false}
        ref={scrollRef}>
        {getScrollViewContent({})}
      </ScrollView>
    </View>
  );
}

const styles = StyleSheet.create({
  wrapperView: {
    height: 300,
    width: '60%',
  },
});