import {TestSuite} from '@rnoh/testerino';
import React from 'react';
import {StyleSheet, View, DevSettings} from 'react-native';
import {Button} from '../components';
import {TestCase} from '../components';

export const DevSettingsTest = () => {
  return (
    <TestSuite name="DevSettings">
      <TestCase.Manual
        itShould="add custom DevMenu item and react to presses after opening DevMenu"
        initialState={false}
        arrange={({setState}) => <AddMenuItemTest setState={setState} />}
        assert={({state, expect}) => {
          expect(state).to.be.true;
        }}
      />
      <TestCase.Example itShould="reload the app after pressing the button">
        <View style={styles.container}>
          <Button
            label="Reload application"
            onPress={() => DevSettings.reload('Test reload')}
          />
        </View>
      </TestCase.Example>
    </TestSuite>
  );
};

const AddMenuItemTest = (props: {
  setState: React.Dispatch<React.SetStateAction<boolean>>;
}) => {
  const customMenuItemOnPress = () => {
    props.setState(true);
  };
  const addCustomMenuItem = () => {
    DevSettings.addMenuItem('Custom menu item', customMenuItemOnPress);
  };
  return (
    <View style={styles.container}>
      <Button label="Add custom menu item" onPress={addCustomMenuItem} />
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    height: 50,
    justifyContent: 'center',
  },
});
