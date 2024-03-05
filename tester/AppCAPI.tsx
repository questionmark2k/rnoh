import {useEffect, useState} from 'react';
import {View, Text, ScrollView} from 'react-native';

export function AppCAPI() {
  const [isFlagOn, setIsFlagOn] = useState(false);

  useEffect(() => {
    const intervalId = setInterval(() => {
      setIsFlagOn(prev => !prev);
    }, 1000);
    return () => {
      clearTimeout(intervalId);
    };
  }, []);

  return (
    <ScrollView style={{backgroundColor: 'silver'}}>
      <View collapsable={false} style={{height: 1000}}>
        <View
          style={{backgroundColor: 'rgba(255,0,0,1)', width: 100, height: 100}}
        />
        <View
          style={{backgroundColor: 'rgba(0,255,0,1)', width: 100, height: 100}}
          collapsable={false}>
          {isFlagOn && <Text>Hello world!</Text>}
        </View>
        <View
          style={{backgroundColor: 'rgba(0,0,255,1)', width: 100, height: 100}}
        />
      </View>
    </ScrollView>
  );
}
