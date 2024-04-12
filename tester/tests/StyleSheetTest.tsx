import {StyleSheet} from 'react-native';
import {TestSuite} from '@rnoh/testerino';
import {TestCase} from '../components';

export const StyleSheetTest = () => {
  return (
    <TestSuite name="Stylesheet">
      <TestCase.Logical
        tags={['C_API']}
        itShould="see if StyleSheet.create returns reference to an object with the correct props"
        fn={({expect}) => {
          expect(styles.test).to.be.eql(testStyle);
        }}
      />
      <TestCase.Logical
        tags={['C_API']}
        itShould="compose styles correctly"
        fn={({expect}) => {
          const composed = StyleSheet.compose(testStyle, testStyle2);
          expect(composed).to.be.an('array');
          expect(composed).to.include(testStyle);
          expect(composed).to.include(testStyle2);
        }}
      />
      <TestCase.Logical
        tags={['C_API']}
        itShould="flatten styles correctly"
        fn={({expect}) => {
          expect(StyleSheet.flatten([testStyle, testStyle2])).to.be.eql(
            flattenedStyle,
          );
        }}
      />
      <TestCase.Logical
        tags={['C_API']}
        itShould="return a sensible hairlineWidth value"
        fn={({expect}) => {
          expect(StyleSheet.hairlineWidth).to.be.greaterThan(0).and.lessThan(2);
        }}
      />
    </TestSuite>
  );
};
const testStyle = {
  color: 'red',
  width: 20,
};
const testStyle2 = {
  color: 'blue',
  height: 30,
};
const flattenedStyle = {
  color: 'blue',
  width: 20,
  height: 30,
};
const styles = StyleSheet.create({
  test: testStyle,
});
