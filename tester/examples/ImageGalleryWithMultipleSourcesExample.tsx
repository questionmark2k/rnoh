import React, {useState} from 'react';
import {Image, ScrollView, StyleSheet, TextInput, View} from 'react-native';
const REMOTE_IMAGE_URL =
  'https://images.pexels.com/photos/7125778/pexels-photo-7125778.jpeg?auto=compress&cs=tinysrgb&w=2000&h=2000&dpr=2';

// run example and it will break, sometimes after scrolling the page
export function ImageGalleryWithMultipleSourcesExample() {
  const [numberOfComponents, setNumberOfComponents] = useState(70);

  return (
    <ScrollView style={{flex: 1}}>
      <View style={{flexDirection: 'column'}}>
        <TextInput
          style={styles.textInput}
          value={numberOfComponents.toString()}
          onChangeText={value => {
            setNumberOfComponents(parseInt(value) || 0);
          }}
        />
        <View style={styles.gallery}>
          {new Array(numberOfComponents).fill(0).map((_, idx) => {
            return (
              <Image
                key={idx}
                source={{
                  uri: REMOTE_IMAGE_URL + '&v=' + idx,
                }}
                style={styles.image}
              />
            );
          })}
          <Image source={{uri: REMOTE_IMAGE_URL}} style={styles.image} />
          <Image source={{uri: REMOTE_IMAGE_URL}} style={styles.image} />
          <Image source={{uri: REMOTE_IMAGE_URL}} style={styles.image} />
        </View>
      </View>
    </ScrollView>
  );
}
const styles = StyleSheet.create({
  image: {
    width: 30,
    height: 30,
    aspectRatio: 1,
  },
  textInput: {
    backgroundColor: 'white',
    width: '100%',
    height: 36,
    color: 'black',
  },
  gallery: {
    width: '100%',
    flexWrap: 'wrap',
    flexDirection: 'row',
  },
});
