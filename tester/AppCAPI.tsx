import {View, Text} from 'react-native';

export function AppCAPI() {
  return (
    <View collapsable={false}>
      <View
        style={{backgroundColor: 'rgba(255,0,0,1)', width: 100, height: 100}}
      />
      <View
        style={{backgroundColor: 'rgba(0,255,0,1)', width: 100, height: 100}}
        collapsable={false}>
        <Text>Hello world!</Text>
      </View>
      <View
        style={{backgroundColor: 'rgba(0,0,255,1)', width: 100, height: 100}}
      />
    </View>
  );
}
