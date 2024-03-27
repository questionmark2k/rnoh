import {Text, View, Button} from 'react-native';
import {TestSuite, TestCase} from '@rnoh/testerino';
import {SAMPLE_PARAGRAPH_TEXT} from './fixtures';
import {useState} from 'react';

export function TextMeasuringTest() {
  return (
    <TestSuite name="Text">
      <TestSuite name="text measuring">
        <TestCase tags={['C_API']} itShould="not wrap any text">
          <View
            style={{
              backgroundColor: 'yellow',
              width: '100%',
              height: 40,
              display: 'flex',
              flexDirection: 'row',
              flexWrap: 'wrap',
            }}>
            {[
              {
                sortCode: '0',
                sortName: '综合',
              },
              {
                sortCode: '2',
                sortName: '最新',
              },
              {
                sortCode: '3',
                sortName: '评论',
              },
              {
                sortCode: '1',
                sortName: '价格',
              },
            ].map((item, index) => (
              <View
                style={{width: '20%', flexDirection: 'column'}}
                key={item.sortCode + '_' + index}>
                <View
                  style={{
                    width: '100%',
                    height: 40,
                    justifyContent: 'center',
                    alignItems: 'center',
                    flexDirection: 'row',
                  }}>
                  <Text
                    ellipsizeMode={'tail'}
                    numberOfLines={1}
                    style={{fontSize: 14, lineHeight: 19}}>
                    {item.sortName}
                  </Text>
                </View>
              </View>
            ))}
          </View>
        </TestCase>
        <TestCase tags={['C_API']} itShould="display all texts in one line">
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{backgroundColor: 'cyan'}}>
              {'0:12'}
              {'场'}
            </Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{backgroundColor: 'pink'}}>
              {'0;12'}
              {'场'}
            </Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{backgroundColor: 'cyan'}}>
              {'0.12'}
              {'场'}
            </Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{backgroundColor: 'pink'}}>
              {'0,12'}
              {'场'}
            </Text>
          </View>
        </TestCase>
        <TestCase
          tags={['C_API']}
          itShould="display: 'FOO''BAR' next to each other">
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{height: '100%', backgroundColor: 'pink'}}>FOO</Text>
            <Text style={{height: '100%', backgroundColor: 'cyan'}}>BAR</Text>
          </View>
        </TestCase>
        <TestCase tags={['C_API']} itShould="display: 'FOO''BAR' in two lines">
          <View
            style={{
              width: 32,
              flexDirection: 'row',
              borderWidth: 1,
            }}>
            <Text style={{height: '100%', backgroundColor: 'pink'}}>
              FOO BAR
            </Text>
          </View>
        </TestCase>
        <TestCase
          tags={['C_API']}
          itShould="display: 'FOO''BAR' next to each other with different fonts">
          <View
            style={{height: 32, alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text
              style={{
                height: '100%',
                backgroundColor: 'pink',
                fontFamily: 'StintUltraCondensed-Regular',
              }}>
              FOO
            </Text>
            <Text
              style={{
                height: '100%',
                backgroundColor: 'pink',
                fontFamily: 'Pacifico-Regular',
              }}>
              BAR
            </Text>
          </View>
        </TestCase>
        <TestCase
          tags={['C_API']}
          itShould="display: 'FOO''BAR' next to each other with different letterSpacing">
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text
              style={{
                height: '100%',
                backgroundColor: 'pink',
                letterSpacing: 8,
              }}>
              FOO
            </Text>
            <Text
              style={{
                height: '100%',
                backgroundColor: 'cyan',
                letterSpacing: 4,
              }}>
              BAR
            </Text>
          </View>
        </TestCase>
        <TestCase
          tags={['C_API']}
          itShould="text should not exceed cyan background (measuring text on cpp side)">
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{backgroundColor: 'silver'}}>FONT SIZE</Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{fontSize: 20, backgroundColor: 'cyan'}}>
              FONT SIZE
            </Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{backgroundColor: 'silver'}}>FONT WEIGHT</Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{fontWeight: 'bold', backgroundColor: 'cyan'}}>
              FONT WEIGHT
            </Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{backgroundColor: 'silver'}}>LETTER SPACING!</Text>
          </View>
          {/* On Android letter spacing may cause the bounding box to be full width (remove ! to see the problem) */}
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{letterSpacing: 1, backgroundColor: 'cyan'}}>
              LETTER SPACING!
            </Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{backgroundColor: 'silver'}}>
              NUMBER OF LINES @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @
              @ @ @
            </Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text
              style={{width: 256, backgroundColor: 'cyan'}}
              numberOfLines={1}>
              NUMBER OF LINES @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @ @
              @ @ @
            </Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text style={{backgroundColor: 'silver'}}>LINE HEIGHT</Text>
          </View>
          <View style={{alignSelf: 'flex-start', flexDirection: 'row'}}>
            <Text
              style={{lineHeight: 32, backgroundColor: 'cyan'}}
              numberOfLines={1}>
              LINE HEIGHT
            </Text>
          </View>
        </TestCase>
        <TestSuite name="views in text">
          <TestCase tags={['C_API']} itShould="vertically align text and view">
            <Text style={{color: 'red', lineHeight: 82}}>
              {'Hello World'}
              <View style={{width: 12, height: 12, backgroundColor: 'blue'}} />
            </Text>
          </TestCase>
          <TestCase
            tags={['C_API']}
            itShould="not crash when a view is trimmed by number of lines (#1)">
            <Text numberOfLines={1}>
              '中文测试中文测试中文测试中文测试中文测试中文测试中文测试中文测试'
              {<View style={{width: 64, height: 64, backgroundColor: 'red'}} />}
            </Text>
          </TestCase>
          <TestCase
            tags={['C_API']}
            itShould="not crash when a view is trimmed by number of lines (#2)">
            <Text numberOfLines={1}>
              {'中文测试中文测试'}
              {<View style={{width: 308, height: 5, backgroundColor: 'red'}} />}
            </Text>
          </TestCase>
          <TestCase
            tags={['C_API']}
            itShould="wrap first and second paragraph in the same way">
            <View style={{width: 200, backgroundColor: 'silver'}}>
              <Text>
                天地玄黄 宇宙洪荒 日月盈昃 辰宿列张 寒来暑往 秋收冬藏 闰馀成岁
              </Text>
              <Text style={{marginTop: 16}}>
                <Text style={{fontWeight: 'bold'}}>天地玄黄</Text> 宇宙洪荒
                日月盈昃 辰宿列张 寒来暑往 秋收冬藏 闰馀成岁
              </Text>
            </View>
          </TestCase>
          <TestCase tags={['C_API']} itShould="not crash the app">
            <Text>
              <View style={{width: 64, height: 64, backgroundColor: 'red'}} />
            </Text>
          </TestCase>
          <TestCase
            tags={['C_API']}
            itShould="render red rectangle after 'FOO'">
            <View
              style={{
                height: 32,
                alignSelf: 'flex-start',
                flexDirection: 'row',
              }}>
              <Text style={{height: '100%', backgroundColor: 'pink'}}>
                FOO
                <View style={{width: 32, height: 16, backgroundColor: 'red'}} />
                BAR
              </Text>
            </View>
          </TestCase>
          <TestCase
            tags={['C_API']}
            itShould="render view in first line and text in second">
            <View
              style={{
                width: 256,
                borderWidth: 1,
              }}>
              <Text numberOfLines={2}>
                <View
                  style={{
                    width: 200,
                    height: 30,
                    backgroundColor: 'red',
                  }}
                />
                test12345678901234567890
              </Text>
            </View>
          </TestCase>
          <TestCase
            tags={['C_API']}
            itShould="[fails on Android/Harmony] render red rectangle after 'FOO' (flex)">
            <View
              style={{
                height: 32,
                alignSelf: 'flex-start',
                flexDirection: 'row',
              }}>
              <Text
                style={{
                  height: '100%',
                  backgroundColor: 'pink',
                  flexDirection: 'row',
                }}>
                FOO
                <View style={{flex: 1, height: 32, backgroundColor: 'red'}} />
                BAR
              </Text>
            </View>
          </TestCase>
          <TestCase
            tags={['C_API']}
            itShould="[buggy on Android/Harmony] render red rectangle after 'FOO' (width: 50%, height: 50%)">
            <View
              style={{
                height: 32,
                alignSelf: 'flex-start',
                flexDirection: 'row',
              }}>
              <Text style={{height: '100%', backgroundColor: 'pink'}}>
                FOO
                <View
                  style={{width: '50%', height: '50%', backgroundColor: 'red'}}
                />
                BAR
              </Text>
            </View>
          </TestCase>
          <TestCase
            tags={['C_API']}
            itShould="render red rectangle with 'BAZ' inside after 'FOO'">
            <View
              style={{
                height: 32,
                alignSelf: 'flex-start',
                flexDirection: 'row',
              }}>
              <Text style={{height: '100%', backgroundColor: 'pink'}}>
                FOO
                <View style={{backgroundColor: 'red'}}>
                  <Text>BAZ</Text>
                </View>
                BAR
              </Text>
            </View>
          </TestCase>
        </TestSuite>
        <TestCase
          tags={['C_API']}
          itShould="show a long text without a space below or above">
          <Text>{SAMPLE_PARAGRAPH_TEXT}</Text>
        </TestCase>
        <TestCase
          tags={['C_API']}
          itShould="show a long text without a space below or above (fontFamily)">
          <Text style={{fontFamily: 'StintUltraCondensed-Regular'}}>
            {SAMPLE_PARAGRAPH_TEXT}
          </Text>
        </TestCase>
        <TestCase
          tags={['C_API']}
          itShould="show a long text without a space below or above (font size)">
          <Text style={{fontSize: 8}}>{SAMPLE_PARAGRAPH_TEXT}</Text>
        </TestCase>
        <TestCase
          tags={['C_API']}
          itShould="show a long text without a space below or above (line height)">
          <Text style={{lineHeight: 21}}>{SAMPLE_PARAGRAPH_TEXT}</Text>
        </TestCase>
        <TestCase tags={['C_API']} itShould="show 2 lines of text">
          <Text numberOfLines={2}>{SAMPLE_PARAGRAPH_TEXT}</Text>
        </TestCase>
        <TestCase tags={['C_API']} itShould="click to update numberOfLines">
          <TextUpdateNumberOfLinesTest />
        </TestCase>
        <TestCase
          tags={['C_API']}
          itShould="show text without a space below or above (fragments)">
          <Text>
            <Text>
              Nostrud irure ex sunt dolor [\n]{'\n'}cillum irure laboris ex ut
              adipisicing magna reprehenderit Lorem.
            </Text>
            <Text style={{fontSize: 24}}>
              Do ullamco excepteur quis labore Lorem mollit tempor ex minim.
            </Text>
            <Text>
              Excepteur consequat officia ut incididunt consectetur qui
              reprehenderit quis quis ut cillum ad.
            </Text>
          </Text>
        </TestCase>
        <TestCase
          tags={['C_API']}
          itShould="show text without a space below or above (fragments with different fonts)"
          skip
          //https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/564
          //https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/565
        >
          <Text>
            <Text style={{fontFamily: 'StintUltraCondensed-Regular'}}>
              Nostrud irure ex sunt dolor [\n]{'\n'}cillum irure laboris ex ut
              adipisicing magna reprehenderit Lorem.
            </Text>
            <Text style={{fontSize: 24, fontFamily: 'Pacifico-Regular'}}>
              Do ullamco excepteur quis labore Lorem mollit tempor ex minim.
            </Text>
            <Text>
              Excepteur consequat officia ut incididunt consectetur qui
              reprehenderit quis quis ut cillum ad.
            </Text>
          </Text>
        </TestCase>
      </TestSuite>
    </TestSuite>
  );
}

const TextUpdateNumberOfLinesTest = () => {
  const [maxLines, setMaxLines] = useState(1);
  return (
    <View>
      <Button
        title="Click"
        onPress={() => {
          setMaxLines(prev => (prev % 8) + 1);
        }}
      />
      <Text numberOfLines={maxLines}>{SAMPLE_PARAGRAPH_TEXT}</Text>
    </View>
  );
};
