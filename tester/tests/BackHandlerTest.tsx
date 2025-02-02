import {BackHandler, Text, TouchableOpacity, View} from 'react-native';
import {TestSuite} from '@rnoh/testerino';
import {useCallback, useState} from 'react';
import {Button, TestCase} from '../components';

export const BackHandlerTest = () => {
  const [counter, setCounter] = useState(0);

  const handleBackPress = useCallback(() => {
    setCounter(prev => prev + 1);
    return true;
  }, []);
  return (
    <TestSuite name="BackHandler">
      <TestCase.Logical
        itShould="be exported"
        fn={({expect}) => {
          expect(BackHandler).to.be.not.undefined;
        }}
      />
      <TestCase.Example itShould="exit app on press">
        <TouchableOpacity
          style={{height: 64}}
          onPress={() => {
            BackHandler.exitApp();
          }}>
          <Text style={{width: '100%', height: '100%'}}>Exit</Text>
        </TouchableOpacity>
      </TestCase.Example>
      <TestCase.Example itShould="allow to add, remove eventListener and display number of system back presses/gestures accordingly">
        <Text style={{width: '100%'}}>
          Back pressed {counter} time{counter === 1 ? '' : 's'}
        </Text>
        <View style={{flexDirection: 'row'}}>
          <Button
            label={'Add event listener'}
            onPress={() => {
              BackHandler.addEventListener(
                'hardwareBackPress',
                handleBackPress,
              );
            }}
          />
          <Button
            label={'Remove event listener'}
            onPress={() => {
              BackHandler.removeEventListener(
                'hardwareBackPress',
                handleBackPress,
              );
            }}
          />
        </View>
      </TestCase.Example>
    </TestSuite>
  );
};
