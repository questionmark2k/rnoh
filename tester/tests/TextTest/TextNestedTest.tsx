import {StyleSheet, Text, View} from 'react-native';
import {TestSuite, TestCase} from '@rnoh/testerino';
import {Button} from '../../components';
import {useState} from 'react';
import {SAMPLE_PARAGRAPH_TEXT} from './fixtures';

export function TextNestedTest() {
  return (
    <TestSuite name="nested texts">
      <TestCase
        tags={['C_API']}
        itShould="show INNER and OUTER texts on the same height (various lineHeights)">
        <View
          style={{
            flexDirection: 'row',
          }}>
          <Text style={{lineHeight: 20, backgroundColor: 'green'}}>
            <Text style={{lineHeight: 25, backgroundColor: 'yellow'}}>
              INNER
            </Text>
            OUTER
          </Text>
        </View>
      </TestCase>
      <TestCase
        tags={['C_API']}
        itShould="show text with ellipsize at the end of the first line">
        <Text ellipsizeMode="tail" numberOfLines={1}>
          Cupidatat irure velit id consequat magna irure quis laborum aute anim
          est cillum aliqua dolor.
        </Text>
      </TestCase>
      <TestCase
        tags={['C_API']}
        itShould="use green background color for INNER (backgroundColor in text fragments)">
        <View
          style={{
            flexDirection: 'row',
          }}>
          <Text style={{backgroundColor: 'red'}}>
            <Text style={{backgroundColor: 'green'}}>INNER</Text>
            OUTER
          </Text>
        </View>
      </TestCase>
      <TestCase
        tags={['C_API']}
        itShould="show text with different vertical alignments (verticalAlign)">
        <View style={{...styles.smallContainerRow}}>
          <Text style={{verticalAlign: 'auto'}}>
            -<Text style={styles.blueShortText}>Auto</Text>-
          </Text>
          <Text style={{verticalAlign: 'top'}}>
            -<Text style={styles.blueShortText}>Top</Text>-
          </Text>
          <Text style={{verticalAlign: 'middle'}}>
            -<Text style={styles.blueShortText}>Middle</Text>-
          </Text>
          <Text style={{verticalAlign: 'bottom'}}>
            -<Text style={styles.blueShortText}>Bottom</Text>-
          </Text>
        </View>
      </TestCase>
      <TestCase tags={['C_API']} itShould="show text with different fontStyles">
        <View style={{...styles.smallContainerRow}}>
          <Text style={{fontStyle: 'normal'}}>
            <Text style={styles.blueShortText}>Normal</Text>
          </Text>
          <Text style={{fontStyle: 'italic'}}>
            <Text style={styles.blueShortText}>Top</Text>
          </Text>
        </View>
      </TestCase>
      <TestCase
        tags={['C_API']}
        itShould="show text with different text decorations">
        <View style={{...styles.smallContainerRow}}>
          <Text
            style={{
              textDecorationLine: 'line-through',
              textDecorationColor: 'green',
            }}>
            <Text style={styles.blueShortText}>Green line-through</Text>
          </Text>
          <Text
            style={{
              textDecorationLine: 'underline',
              textDecorationColor: 'blue',
            }}>
            <Text style={styles.blueShortText}>Blue underline</Text>
          </Text>
        </View>
      </TestCase>
      <TestCase
        tags={['C_API']}
        itShould="show text with shadow"
        skip
        //https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/278
      >
        <View style={styles.smallContainer}>
          <Text
            style={{
              textShadowColor: 'rgba(0,0,255,0.8)',
              textShadowOffset: {width: 1, height: 1},
              textShadowRadius: 20,
            }}>
            <Text style={styles.smallText}>Text with shadow</Text>
          </Text>
        </View>
      </TestCase>
      <TestCase
        tags={['C_API']}
        itShould="show text with correct textTransform "
        //  123 1one is added to the end of text to see if the code correctly handles number
      >
        <View style={styles.bigContainer}>
          <Text style={styles.smallText}>Text transform none 123 1one</Text>
          <Text
            style={{
              textTransform: 'capitalize',
            }}>
            <Text style={styles.blueShortText}>
              Text transform capitalize 123 1one
            </Text>
          </Text>
          <Text
            style={{
              textTransform: 'uppercase',
            }}>
            <Text style={styles.blueShortText}>
              Text transform uppercase 123 1one
            </Text>
          </Text>
          <Text
            style={{
              textTransform: 'lowercase',
            }}>
            <Text style={styles.blueShortText}>
              Text transform lowercase 123 1one
            </Text>
          </Text>
        </View>
      </TestCase>
      <TestCase
        tags={['C_API']}
        itShould="show text with different vertical alignments (textAlignVertical)">
        <View style={{...styles.smallContainerRow}}>
          <Text style={{textAlignVertical: 'auto'}}>
            -<Text style={styles.blueShortText}>Auto</Text>-
          </Text>
          <Text style={{textAlignVertical: 'top'}}>
            -<Text style={styles.blueShortText}>Top</Text>-
          </Text>
          <Text style={{textAlignVertical: 'center'}}>
            -<Text style={styles.blueShortText}>Center</Text>-
          </Text>
          <Text style={{textAlignVertical: 'bottom'}}>
            -<Text style={styles.blueShortText}>Bottom</Text>-
          </Text>
        </View>
      </TestCase>
      <TestCase tags={['C_API']} itShould="align text vertically">
        <View style={{width: 300, borderRightWidth: 1}}>
          <Text>
            <Text style={{lineHeight: 64, backgroundColor: 'red'}}>
              lineHeight: 64
            </Text>
            <Text style={{lineHeight: 16, backgroundColor: 'blue'}}>
              lineHeight: 16; lineHeight: 16; lineHeight: 16; lineHeight: 16;
            </Text>
            <View style={{backgroundColor: 'purple', width: 16, height: 16}} />
          </Text>
          <Text style={{marginTop: 16}}>
            <Text style={{lineHeight: 16, backgroundColor: 'blue'}}>
              lineHeight: 16
            </Text>
            <Text style={{lineHeight: 64, backgroundColor: 'red'}}>
              lineHeight: 64; lineHeight: 64; lineHeight: 64; lineHeight: 64
            </Text>
            <View style={{backgroundColor: 'purple', width: 16, height: 16}} />
          </Text>

          <Text style={{marginTop: 16}}>
            <Text
              style={{
                lineHeight: 64,
                backgroundColor: 'red',
                verticalAlign: 'bottom',
              }}>
              lineHeight: 16; verticalAlign: bottom; lineHeight: 16;
              verticalAlign: bottom;
            </Text>
            <Text
              style={{
                lineHeight: 16,
                backgroundColor: 'blue',
                verticalAlign: 'bottom',
              }}>
              lineHeight: 16; verticalAlign: bottom;
            </Text>
          </Text>
        </View>
      </TestCase>
      <TestCase
        tags={['C_API']}
        itShould="show text with different horizontal alignments">
        <Text style={{textAlign: 'left'}}>
          <Text>Left: </Text>
          <Text style={{fontSize: 8}}>{SAMPLE_PARAGRAPH_TEXT}</Text>
        </Text>
        <Text style={{textAlign: 'center'}}>
          <Text>Center: </Text>
          <Text style={{fontSize: 8}}>{SAMPLE_PARAGRAPH_TEXT}</Text>
        </Text>
        <Text style={{textAlign: 'right'}}>
          <Text>Right: </Text>
          <Text style={{fontSize: 8}}>{SAMPLE_PARAGRAPH_TEXT}</Text>
        </Text>
        <Text style={{textAlign: 'justify'}}>
          <Text>Justify: </Text>
          <Text style={{fontSize: 8}}>{SAMPLE_PARAGRAPH_TEXT}</Text>
        </Text>
      </TestCase>
      <TestCase tags={['C_API']} itShould="display 1 line of text">
        <View style={{width: 200, backgroundColor: 'silver'}}>
          <Text style={{textAlign: 'left'}} numberOfLines={1}>
            <Text style={{fontSize: 12, backgroundColor: 'cyan'}}>{'>'}</Text>
            <Text style={{fontSize: 8}}>{SAMPLE_PARAGRAPH_TEXT}</Text>
          </Text>
        </View>
      </TestCase>
      <TestCase tags={['C_API']} itShould="display 2 lines of text">
        <Text style={{textAlign: 'left'}} numberOfLines={2}>
          <Text style={{fontSize: 12, backgroundColor: 'cyan'}}>{'@@@@@'}</Text>
          <Text style={{fontSize: 8}}>{SAMPLE_PARAGRAPH_TEXT}</Text>
        </Text>
      </TestCase>
      <TestCase
        tags={['C_API']}
        itShould="display 2 lines of text (placeholder test)">
        <Text style={{textAlign: 'left'}} numberOfLines={2}>
          <View style={{width: 0, height: 8, backgroundColor: 'red'}} />
          <Text style={{fontSize: 8}}>{SAMPLE_PARAGRAPH_TEXT}</Text>
        </Text>
      </TestCase>
      <TestCase tags={['C_API']} itShould="wrap long words">
        <View style={{backgroundColor: 'silver', width: 200}}>
          <Text style={{textAlign: 'left'}}>
            <View style={{width: 8, height: 8, backgroundColor: 'red'}} />
            <Text style={{fontSize: 8}}>
              0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
            </Text>
          </Text>
        </View>
      </TestCase>
      <TestCase tags={['C_API']} itShould="text should be disabled">
        <TextDisabledTest />
      </TestCase>
      <TestCase
        tags={['C_API']}
        modal
        itShould="show justified text (manually added spaces)">
        <Text
          style={{
            textAlign: 'justify',
            backgroundColor: 'green',
          }}>
          <Text style={{fontSize: 16}}>Justify te st</Text>
          <Text style={{fontSize: 8, backgroundColor: 'purple'}}>
            Quis exercitation do eu in laboris nulla sit elit officia.
            Incididunt ipsum aliquip IncididuntIncididunt commodo proident
          </Text>
          <Text style={{fontSize: 38, backgroundColor: 'blue'}}>
            Quis exercitation do eu in laboris nulla sit elit officia.
            Incididunt ipsum aliquip IncididuntIncididunrumbumlublum commodo
            proident
          </Text>
        </Text>
      </TestCase>
      <TestCase
        tags={['C_API']}
        modal
        itShould="activate onPress() on <Text /> if the touch moves up to 100px above or below the <Text /> body">
        <TextPressRetentionOffsetTest />
      </TestCase>
      <TestCase
        tags={['C_API']}
        modal
        itShould="increase the counter when 'press me' is pressed (handling gestures in text fragments)">
        <TextPressNestedTest />
      </TestCase>
    </TestSuite>
  );
}

const TextDisabledTest = () => {
  const [pressCount, setPressCount] = useState(0);
  const [disabled, setDisabled] = useState(false);

  return (
    <View>
      <Text> Press count: {pressCount} </Text>
      <Text disabled={disabled} onPress={() => setPressCount(pressCount + 1)}>
        {SAMPLE_PARAGRAPH_TEXT}
      </Text>
      <Button
        label={disabled ? 'Enable Text' : 'Disable Text'}
        onPress={() => setDisabled(!disabled)}
      />
    </View>
  );
};

const TextPressRetentionOffsetTest = () => {
  const [pressCount, setPressCount] = useState(0);

  return (
    <View style={{height: 500, justifyContent: 'center'}}>
      <Text style={{textAlign: 'center'}}> Press count: {pressCount} </Text>
      <View
        style={{
          backgroundColor: 'lightblue',
          height: 250,
          justifyContent: 'center',
        }}>
        <Text
          style={{
            backgroundColor: 'blue',
            height: 50,
            textAlign: 'center',
            textAlignVertical: 'center',
            color: 'white',
          }}
          onPress={() => setPressCount(pressCount + 1)}
          // @ts-ignore - pressRetentionOffset is not in the type definition
          pressRetentionOffset={{
            bottom: 100,
            left: 0,
            right: 0,
            top: 100,
          }}>
          pressRetentionOffset
        </Text>
      </View>
    </View>
  );
};

const TextPressNestedTest = () => {
  const [pressCount, setPressCount] = useState(0);

  return (
    <View style={{height: 500, justifyContent: 'center'}}>
      <Text style={{textAlign: 'center'}}> Press count: {pressCount} </Text>
      <View
        style={{
          backgroundColor: 'lightblue',
          height: 250,
          justifyContent: 'center',
        }}>
        <Text style={{backgroundColor: 'green', height: 250}}>
          <Text
            style={{backgroundColor: 'purple', height: 150, fontSize: 34}}
            onPress={() => setPressCount(pressCount + 1)}>
            press me (on android press below also triggers event)
          </Text>
        </Text>
      </View>
    </View>
  );
};

const styles = StyleSheet.create({
  smallContainer: {
    width: 200,
    height: 40,
    backgroundColor: 'red',
  },
  smallContainerRow: {
    width: 200,
    height: 40,
    backgroundColor: 'red',
    flexDirection: 'row',
  },
  bigContainer: {
    width: 200,
    height: 120,
    backgroundColor: 'red',
  },
  smallText: {
    height: 30,
    color: 'white',
  },
  blueShortText: {
    height: 30,
    width: 50,
    color: 'white',
    backgroundColor: 'blue',
  },
});
