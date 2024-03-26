import {TestSuite} from '@rnoh/testerino';
import {Button, TestCase} from '../components';
import {Alert, Text, View} from 'react-native';
import {useState} from 'react';

export function AlertTest() {
  return (
    <TestSuite name="Alert">
      <TestCase.Manual
        itShould="show simple alert on click"
        initialState={false}
        arrange={({setState, reset}) => (
          <View style={{flexDirection: 'row'}}>
            <Button
              label="show alert"
              onPress={() => {
                Alert.alert('Test Alert', 'Message', [
                  {text: 'OK', onPress: () => setState(true)},
                ]);
              }}
            />
            <Button label="reset" onPress={reset} />
          </View>
        )}
        assert={({state, expect}) => {
          expect(state).to.be.true;
        }}
      />
      <TestCase.Manual
        itShould="display name of the button clicked in alert"
        initialState={false}
        arrange={({setState, reset}) => (
          <AlertView setState={setState} reset={reset} />
        )}
        assert={({state, expect}) => {
          expect(state).to.be.true;
        }}
      />
      <TestCase.Manual
        itShould="cancel alert on press outside its window"
        initialState={false}
        arrange={({setState, reset}) => (
          <View style={{flexDirection: 'row'}}>
            <Button
              label="show alert"
              onPress={() => {
                Alert.alert('Test Alert', 'Press outside to dismiss', [], {
                  cancelable: true,
                  onDismiss: () => setState(true),
                });
              }}
            />
            <Button label="reset" onPress={reset} />
          </View>
        )}
        assert={({state, expect}) => {
          expect(state).to.be.true;
        }}
      />
      <TestCase.Example itShould="show an alert with OK button (when a developer doesn't declare any buttons)">
        <Button
          label="show alert"
          onPress={() => {
            Alert.alert('Test Alert');
          }}
        />
      </TestCase.Example>
      <TestCase.Manual
        initialState={false}
        itShould="pass after pressing the OK button in the Alert Box"
        arrange={({setState}) => {
          return (
            <Button
              label="show alert"
              onPress={() => {
                Alert.alert('Test Alert', undefined, [
                  {
                    text: 'OK',
                    onPress: () => {
                      setState(true);
                    },
                  },
                ]);
              }}
            />
          );
        }}
        assert={({expect, state}) => {
          expect(state).to.be.true;
        }}
      />
    </TestSuite>
  );
}

const AlertView = (props: {
  setState: React.Dispatch<React.SetStateAction<boolean>>;
  reset: () => void;
}) => {
  const [text, setText] = useState('');
  const cancelOnPress = () => {
    props.setState(true);
    setText('Cancel');
  };
  const okOnPress = () => {
    props.setState(true);
    setText('OK');
  };
  return (
    <>
      <Text>{text}</Text>
      <View style={{flexDirection: 'row'}}>
        <Button
          label="show alert"
          onPress={() => {
            Alert.alert('Test Alert', 'Message', [
              {text: 'Cancel', onPress: cancelOnPress},
              {text: 'OK', onPress: okOnPress},
            ]);
          }}
        />
        <Button label="reset" onPress={props.reset} />
      </View>
    </>
  );
};
