import {TestCase, TestSuite} from '@rnoh/testerino';
import React, {useState} from 'react';
import {
  Modal,
  StyleSheet,
  Text,
  View,
  ModalProps,
  TextInput,
  FlatList,
} from 'react-native';
import {Button} from '../components';

export function ModalTest() {
  return (
    <TestSuite name="Modal">
      <TestCase itShould="show modal">
        <ModalExample />
      </TestCase>
      <TestCase
        itShould="trigger onShow event after modal is shown"
        initialState={false}
        arrange={({setState}) => (
          <ModalExample
            onShow={() => {
              setState(true);
            }}
          />
        )}
        assert={({state, expect}) => {
          expect(state).to.be.true;
        }}
      />
      <TestCase
        itShould="show modal with slide animation"
        initialState={false}
        arrange={({setState}) => (
          <ModalExample
            animationType="slide"
            onShow={() => {
              setState(true);
            }}
          />
        )}
        assert={({state, expect}) => {
          expect(state).to.be.true;
        }}
      />
      <TestCase
        itShould="show modal with fade animation"
        initialState={false}
        arrange={({setState}) => (
          <ModalExample
            animationType="fade"
            onShow={() => {
              setState(true);
            }}
          />
        )}
        assert={({state, expect}) => {
          expect(state).to.be.true;
        }}
      />
      <TestCase itShould="soft keyboard should cover modal">
        <ModalExample withTextInput />
      </TestCase>
      <TestCase itShould="allow to display one modal on top of another">
        <NestedModalinModal />
      </TestCase>
      <TestCase itShould="display modal from flatlist item">
        <NestedModalInFlatlist />
      </TestCase>
    </TestSuite>
  );
}

const ModalExample = (props: ModalProps & {withTextInput?: boolean}) => {
  const [modalVisible, setModalVisible] = useState(false);
  return (
    <View>
      <Text style={[styles.textStyle, {color: 'black'}]}>
        {modalVisible ? 'shown' : 'hidden'}
      </Text>
      <Modal
        {...props}
        transparent={true}
        visible={modalVisible}
        onRequestClose={() => {
          setModalVisible(!modalVisible);
        }}>
        <View style={styles.centeredView}>
          <View style={styles.modalView}>
            <Text style={styles.modalText}>Hello World!</Text>
            {props.withTextInput && <TextInput style={styles.textInput} />}
            <Button label="Hide Modal" onPress={() => setModalVisible(false)} />
          </View>
        </View>
      </Modal>
      <Button label="Show Modal" onPress={() => setModalVisible(true)} />
    </View>
  );
};

const NestedModalinModal = () => {
  const [firstModalVisible, setFirstModalVisible] = useState(false);
  const [secondModalVisible, setSecondModalVisible] = useState(false);

  return (
    <View>
      <Text style={[styles.textStyle, {color: 'black'}]}>
        First: {firstModalVisible ? 'shown' : 'hidden'}
      </Text>
      <Text style={[styles.textStyle, {color: 'black'}]}>
        Second: {secondModalVisible ? 'shown' : 'hidden'}
      </Text>

      <Modal
        animationType="fade"
        transparent={true}
        visible={firstModalVisible}
        onRequestClose={() => {
          setFirstModalVisible(!firstModalVisible);
        }}>
        <View style={styles.centeredView}>
          <View style={[styles.modalView, {padding: 60}]}>
            <Text style={styles.modalText}>Hello World from first Modal!</Text>
            <Modal
              animationType="slide"
              transparent={true}
              visible={secondModalVisible}
              onRequestClose={() => {
                setSecondModalVisible(!secondModalVisible);
              }}>
              <View style={styles.centeredView}>
                <View
                  style={[styles.modalView, {backgroundColor: 'lightblue'}]}>
                  <Text style={styles.modalText}>
                    Hello World from second Modal!
                  </Text>
                  <Button
                    label="Hide Second Modal"
                    onPress={() => setSecondModalVisible(false)}
                  />
                </View>
              </View>
            </Modal>
            <Button
              label="Hide Modal"
              onPress={() => setFirstModalVisible(false)}
            />
            <Button
              label="Open another Modal"
              onPress={() => setSecondModalVisible(true)}
            />
          </View>
        </View>
      </Modal>
      <Button label="Show Modal" onPress={() => setFirstModalVisible(true)} />
    </View>
  );
};

const DATA = [
  {
    id: 'bd7acbea-c1b1-46c2-aed5-3ad53abb28ba',
    title: 'First Item',
  },
  {
    id: '3ac68afc-c605-48d3-a4f8-fbd91aa97f63',
    title: 'Second Item',
  },
  {
    id: '58694a0f-3da1-471f-bd96-145571e29d72',
    title: 'Third Item',
  },
];

type ItemProps = {title: string};

const Item = ({title}: ItemProps) => {
  const [modalVisible, setModalVisible] = useState(false);

  return (
    <View>
      <Modal
        animationType="slide"
        transparent={true}
        visible={modalVisible}
        onRequestClose={() => {
          setModalVisible(!modalVisible);
        }}>
        <View style={styles.centeredView}>
          <View style={styles.modalView}>
            <Text style={styles.modalText}>{title}</Text>
            <Button label="Hide Modal" onPress={() => setModalVisible(false)} />
          </View>
        </View>
      </Modal>
      <View style={{flexDirection: 'row', justifyContent: 'space-between'}}>
        <Text>{title}</Text>
        <Button label="Show Modal" onPress={() => setModalVisible(true)} />
      </View>
    </View>
  );
};

const NestedModalInFlatlist = () => {
  return (
    <>
      <FlatList
        data={DATA}
        renderItem={({item}) => <Item title={item.title} />}
        keyExtractor={item => item.id}
      />
    </>
  );
};

const styles = StyleSheet.create({
  centeredView: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    marginTop: 22,
  },
  modalView: {
    margin: 20,
    backgroundColor: 'white',
    borderRadius: 20,
    padding: 35,
    alignItems: 'center',
    shadowColor: '#000',
    shadowOffset: {
      width: 0,
      height: 2,
    },
    shadowOpacity: 0.25,
    shadowRadius: 4,
    elevation: 5,
  },
  button: {
    borderRadius: 20,
    padding: 10,
    elevation: 2,
  },
  buttonOpen: {
    backgroundColor: '#F194FF',
  },
  buttonClose: {
    backgroundColor: '#2196F3',
  },
  textStyle: {
    color: 'white',
    fontWeight: 'bold',
    textAlign: 'center',
    height: 20,
  },
  modalText: {
    marginBottom: 15,
    textAlign: 'center',
    height: 20,
  },
  textInput: {
    width: 150,
    height: 40,
    backgroundColor: 'rgb(200,200,200)',
    borderRadius: 12,
    marginVertical: 4,
  },
});
