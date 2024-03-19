import {
  KeyboardTypeOptions,
  Platform,
  ReturnKeyType,
  ReturnKeyTypeAndroid,
  StyleSheet,
  Text,
  TextInput,
  TextInputProps,
  View,
} from 'react-native';
import {TestSuite, TestCase} from '@rnoh/testerino';
import {useState, useRef} from 'react';
import {Button, Effect, StateKeeper} from '../components';

const KEYBOARD_TYPES: KeyboardTypeOptions[] = [
  'default',
  'number-pad',
  'decimal-pad',
  'numeric',
  'email-address',
  'phone-pad',
  'url',
];

export function TextInputTest() {
  return (
    <TestSuite name="TextInput">
      <TestCase
        modal
        itShould="render textinput and change the text component based on the values inputted">
        <TextInputWithText style={styles.textInput} />
      </TestCase>
      <TestCase modal itShould="render non-editable textInput">
        <TextInputWithText style={styles.textInput} editable={false} />
      </TestCase>
      <TestCase modal itShould="render textInput with Pacifico Regular font">
        <TextInputWithText
          style={[styles.textInput, {fontFamily: 'Pacifico-Regular'}]}
        />
      </TestCase>
      <TestCase modal itShould="render textInput with caret hidden">
        <TextInputWithText style={styles.textInput} caretHidden />
      </TestCase>
      <TestSuite name="focus/blur">
        <TestCase
          modal
          itShould="blur text on submit (singleline)"
          initialState={false}
          arrange={({setState}) => {
            return (
              <>
                <TextInputWithText
                  style={styles.textInput}
                  blurOnSubmit
                  onBlur={() => setState(true)}
                />
              </>
            );
          }}
          assert={({expect, state}) => {
            expect(state).to.be.true;
          }}
        />
        <TestCase
          modal
          itShould="blur text after switching to another textinput"
          initialState={false}
          arrange={({setState}) => {
            return (
              <>
                <TextInputWithText
                  style={styles.textInput}
                  onBlur={() => setState(true)}
                />
                <TextInputWithText
                  style={styles.textInput}
                  onBlur={() => setState(true)}
                />
              </>
            );
          }}
          assert={({expect, state}) => {
            expect(state).to.be.true;
          }}
        />
        <TestCase
          modal
          itShould="not blur text on submit"
          skip
          //https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/403
        >
          <TextInputWithText style={styles.textInput} blurOnSubmit={false} />
          <TextInputWithText
            style={styles.textInput}
            blurOnSubmit={false}
            multiline
          />
        </TestCase>
        <TestCase
          modal
          itShould="automatically focus textInput when displayed"
          initialState={false}
          arrange={({setState}) => (
            <TextInputWithText
              style={styles.textInput}
              autoFocus
              onFocus={() => setState(true)}
            />
          )}
          assert={({expect, state}) => {
            expect(state).to.be.true;
          }}
        />
        <TestCase
          modal
          itShould="focus textInput on click"
          initialState={false}
          arrange={({setState}) => (
            <TextInput
              style={styles.textInput}
              onFocus={() => setState(true)}
            />
          )}
          assert={({expect, state}) => {
            expect(state).to.be.true;
          }}
        />
        <TestCase
          modal
          itShould="focus textInput when pressing the button"
          initialState={false}
          arrange={({setState}) => <FocusTextInputTest setState={setState} />}
          assert={({state, expect}) => {
            expect(state).to.be.true;
          }}
        />
      </TestSuite>
      <TestCase
        modal
        itShould="render textInput with blue underline"
        skip
        //https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/404
      >
        <TextInputWithText
          style={styles.textInput}
          underlineColorAndroid={'blue'}
        />
      </TestCase>
      <TestCase modal itShould="render textinput with red placeholder">
        <TextInputWithText
          style={styles.textInput}
          placeholder="Placeholder"
          placeholderTextColor={'red'}
        />
      </TestCase>
      <TestCase modal itShould="render textinput with green selection color">
        <TextInputWithText style={styles.textInput} selectionColor="green" />
      </TestCase>
      <TestCase modal itShould="render multiline text input">
        <TextInputWithText style={styles.textInputBigger} multiline />
      </TestCase>
      <TestCase
        modal
        itShould="render multiline text input with Pacifico Regular font">
        <TextInputWithText
          style={[styles.textInputBigger, {fontFamily: 'Pacifico-Regular'}]}
          multiline
        />
      </TestCase>
      <TestCase modal itShould="render text input with maximally 10 characters">
        <TextInputWithText style={styles.textInput} maxLength={10} />
      </TestCase>
      <TestCase modal itShould="toggle between rendering 10 and 5 characters">
        <StateKeeper
          initialValue={10}
          renderContent={(maxLength, setMaxLength) => {
            return (
              <Effect
                onMount={() => {
                  const interval = setInterval(() => {
                    setMaxLength(prev => (prev === 10 ? 5 : 10));
                  }, 1000);
                  return () => {
                    clearInterval(interval);
                  };
                }}>
                <TextInputWithText
                  style={styles.textInput}
                  maxLength={maxLength}
                  value="1234567890"
                />
              </Effect>
            );
          }}
        />
      </TestCase>
      <TestCase
        modal
        itShould="toggle between different capitalization modes"
        skip
        //https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/408
      >
        <AutoCapitalize />
      </TestCase>
      <TestCase
        modal
        itShould="trigger onSubmitEditing event after submiting"
        initialState={false}
        arrange={({setState}) => (
          <TextInputWithText
            style={styles.textInput}
            onSubmitEditing={() => setState(true)}
          />
        )}
        assert={({expect, state}) => {
          expect(state).to.be.true;
        }}
      />
      <TestCase modal itShould="toggle between different return keys">
        <ReturnKeyTypeView />
      </TestCase>
      <TestCase modal itShould="render secure text input (text obscured)">
        <TextInputWithText style={styles.textInput} secureTextEntry />
      </TestCase>
      <TestCase
        modal
        itShould="trigger onKeyPress event after pressing key (press 'A' to pass)"
        initialState={''}
        arrange={({setState}) => (
          <TextInputWithText
            style={styles.textInput}
            autoFocus
            onKeyPress={event => setState(event.nativeEvent.key)}
          />
        )}
        assert={({expect, state}) => {
          expect(state).to.be.eq('A');
        }}
      />
      <TestCase
        modal
        skip
        //https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/736
        itShould="trigger onKeyPress event after pressing backspace"
        initialState={''}
        arrange={({setState}) => (
          <TextInputWithText
            style={styles.textInput}
            autoFocus
            onKeyPress={event => setState(event.nativeEvent.key)}
          />
        )}
        assert={({expect, state}) => {
          expect(state).to.be.eq('Backspace');
        }}
      />
      <TestCase
        modal
        itShould="show text input with default value (defaultProps)">
        <DefaultProps />
      </TestCase>
      <TestCase
        modal
        itShould="show text input with default value (defaultValue)">
        <TextInput style={styles.textInput} defaultValue="defaultText" />
      </TestCase>
      <TestCase
        modal
        itShould="trigger onLayout event on mount"
        initialState={{}}
        arrange={({setState, state}) => {
          return (
            <>
              <Text>{JSON.stringify(state)}</Text>
              <TextInput
                style={styles.textInput}
                onLayout={event => {
                  setState(event.nativeEvent.layout);
                }}
              />
            </>
          );
        }}
        assert={({expect, state}) => {
          expect(state).to.include.all.keys('width', 'height', 'x', 'y');
        }}
      />
      <TestCase
        modal
        itShould="render textinputs with different keyboard types">
        <View>
          {KEYBOARD_TYPES.map(type => (
            <TextInputKeyboardType
              key={`singleline_${type}`}
              keyboardType={type}
            />
          ))}
        </View>
      </TestCase>
      <TestCase
        modal
        itShould="render multiline textinputs with different keyboard types">
        <View>
          {KEYBOARD_TYPES.map(type => (
            <TextInputKeyboardType
              key={`multiline_${type}`}
              keyboardType={type}
              multiline
            />
          ))}
        </View>
      </TestCase>
      <TestCase modal itShould="render textinput with allowFontScaling">
        <TextInputWithText
          style={styles.textInput}
          allowFontScaling
          defaultValue="Scaled"
        />
        <TextInputWithText
          style={styles.textInput}
          allowFontScaling={false}
          defaultValue="Not scaled"
        />
        <TextInputWithText
          style={[styles.textInput, {fontSize: 40}]}
          allowFontScaling={true}
          defaultValue="Scaled big"
        />
        <TextInputWithText
          style={[styles.textInput, {fontSize: 40}]}
          allowFontScaling={false}
          defaultValue="Not scaled big"
        />
      </TestCase>
      <TestCase itShould="show textInput with padding" modal>
        <View style={{width: 300, height: 200}}>
          <TextInputWithText
            style={{
              paddingLeft: 10,
              paddingTop: 20,
              paddingRight: 30,
              paddingBottom: 40,
              backgroundColor: 'red',
            }}
          />
        </View>
      </TestCase>
      <TestCase itShould="show textInput multiline with padding" modal>
        <View style={{width: 300, height: 200}}>
          <TextInputWithText
            style={{
              paddingLeft: 10,
              paddingTop: 20,
              paddingRight: 30,
              paddingBottom: 40,
              backgroundColor: 'red',
              height: 100,
            }}
            multiline
          />
        </View>
      </TestCase>
      <TestCase modal itShould="render textinput with readonly">
        {/* @ts-ignore */}
        <TextInputWithText style={styles.textInput} readOnly />
      </TestCase>
      <TestCase
        modal
        itShould="display bold, italic, large placeholder with a custom font">
        <TextInput
          style={{
            fontFamily: 'StintUltraCondensed-Regular',
            fontWeight: 'bold',
            fontSize: 24,
            fontStyle: 'italic',
          }}
          placeholder="placeholder"
        />
      </TestCase>
      <TestCase modal itShould="render textinput with red text color">
        <TextInputWithText style={[styles.textInput, {color: 'red'}]} />
      </TestCase>
      <TestCase modal itShould="clear text on focus">
        {/* iOS only */}
        <TextInputWithText style={styles.textInput} clearTextOnFocus />
      </TestCase>
      <TestCase modal itShould="use different clearButtonMode values">
        <TextInputWithText style={styles.textInput} clearButtonMode="always" />
        <Text>clearButtonMode="always"</Text>
        {
          /* Except Android */
          Platform.OS !== 'android' && (
            <>
              <TextInputWithText
                style={styles.textInput}
                clearButtonMode="while-editing"
              />
              <Text>clearButtonMode="while-editing"</Text>
              <TextInputWithText
                style={styles.textInput}
                clearButtonMode="unless-editing"
              />
              <Text>clearButtonMode="unless-editing"</Text>
            </>
          )
        }
        <TextInputWithText style={styles.textInput} clearButtonMode="never" />
        <Text>clearButtonMode="never"</Text>
      </TestCase>
      <TestCase modal itShould="show selection start and end values">
        <TextSelectionChangeTest />
      </TestCase>
      <TestCase
        modal
        itShould="show textinput with 'Selection' substring selected">
        <SelectionTest />
      </TestCase>
      <TestCase
        modal
        itShould="not show text context menu when long press">
        <ContextMenuHiddenTest />
      </TestCase>
      <TestCase modal itShould="select text on focus">
        <TextInput
          value="selectTextOnFocus"
          selectTextOnFocus
          style={styles.textInput}
        />
      </TestCase>
    </TestSuite>
  );
}

const SelectionTest = () => {
  return (
    <View>
      <TextInput
        style={styles.textInput}
        value="TextSelectionTest"
        selection={{start: 4, end: 13}}
      />
    </View>
  );
};

const ContextMenuHiddenTest = () => {
  return (
    <View>
      <TextInput
        style={styles.textInput}
        value="ContextMenuHiddenTest"
        contextMenuHidden={true}
      />
      <TextInput
        style={[styles.textInput, {marginTop: 5}]}
        value="ContextMenuHiddenTest_multiline"
        multiline={true}
        contextMenuHidden={true}
      />
    </View>
  );
};

const TextSelectionChangeTest = () => {
  const [eventData, setEventData] = useState('');
  return (
    <View>
      <TextInput
        style={styles.textInput}
        onSelectionChange={event =>
          setEventData(JSON.stringify(event.nativeEvent.selection))
        }
      />
      <Text style={{marginVertical: 8}}>Selection {eventData}</Text>
    </View>
  );
};

const FocusTextInputTest = (props: {
  setState: React.Dispatch<React.SetStateAction<boolean>>;
}) => {
  const ref = useRef<TextInput>(null);
  return (
    <View>
      <Button label="focus text input" onPress={() => ref.current?.focus()} />
      <TextInput onFocus={() => props.setState(true)} ref={ref} />
    </View>
  );
};

const TextInputKeyboardType = (props: TextInputProps) => {
  return (
    <>
      <Text>{props.keyboardType}</Text>
      <TextInput
        style={{...styles.textInputSmall, marginBottom: 10}}
        keyboardType={props.keyboardType}
        multiline={props.multiline}
      />
    </>
  );
};
const TextInputWithText = (props: TextInputProps) => {
  const [text, onChangeText] = useState(props.defaultValue ?? '');
  return (
    <>
      <Text style={styles.text}>{text}</Text>
      <TextInput {...props} onChangeText={onChangeText} value={text} />
    </>
  );
};
type CapitalizationType = 'none' | 'sentences' | 'words' | 'characters';
const AutoCapitalize = () => {
  const [state, setState] = useState<CapitalizationType>('none');
  const capitalizations: Array<CapitalizationType> = [
    'none',
    'sentences',
    'words',
    'characters',
  ];
  const toggleCapitalization = () => {
    const index = capitalizations.indexOf(state);
    setState(capitalizations[(index + 1) % capitalizations.length]);
  };
  return (
    <>
      <TextInputWithText style={styles.textInput} autoCapitalize={state} />
      <Button label="toggle capitalize mode" onPress={toggleCapitalization} />
      <Text>Capitalize mode: {state}</Text>
    </>
  );
};
const ReturnKeyTypeView = () => {
  const [state, setState] = useState<ReturnKeyType | ReturnKeyTypeAndroid>(
    'none',
  );
  const returnKey: Array<ReturnKeyType | ReturnKeyTypeAndroid> = [
    'none',
    'done',
    'go',
    'next',
    'search',
    'send',
    'none',
    'previous', //currently not supported by ArkUI
  ];
  const toggleReturnKey = () => {
    const index = returnKey.indexOf(state);
    setState(returnKey[(index + 1) % returnKey.length]);
  };
  return (
    <>
      <TextInputWithText style={styles.textInput} returnKeyType={state} />
      <Button label="toggle return key type" onPress={toggleReturnKey} />
      <Text>Return key: {state}</Text>
    </>
  );
};

const DefaultProps = () => {
  // @ts-ignore
  TextInput.defaultProps = {
    value: 'defaultText',
  };

  return <TextInput style={styles.textInput} />;
};

const styles = StyleSheet.create({
  container: {
    width: 80,
    height: 80,
    backgroundColor: 'red',
  },
  text: {
    width: '100%',
    height: 40,
  },
  textInputSmall: {
    height: 20, // hack
    fontSize: 8,
    color: 'black',
    backgroundColor: 'rgb(245, 240, 211)',
    borderRadius: 20,
  },
  textInput: {
    height: 40, // hack
    fontSize: 16,
    color: 'black',
    backgroundColor: 'rgb(245, 240, 211)',
    borderRadius: 40,
  },
  textInputBigger: {
    height: 80, // hack
    fontSize: 16,
    color: 'black',
    backgroundColor: 'rgb(245, 240, 211)',
    borderRadius: 20,
  },
});
