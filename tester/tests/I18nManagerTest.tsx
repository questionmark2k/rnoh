import {TestSuite} from '@rnoh/testerino';
import {I18nManager} from 'react-native';
import {TestCase} from '../components';

export function I18nManagerTest() {
  return (
    <TestSuite name="I18nManager">
      <TestCase.Logical
        itShould="be LTR be true"
        fn={({expect}) => {
          expect(I18nManager.isRTL).to.be.false;
        }}
      />
      <TestCase.Logical
        itShould="doLeftAndRightSwapInRTL to be true"
        fn={({expect}) => {
          expect(I18nManager.doLeftAndRightSwapInRTL).to.be.true;
        }}
      />
    </TestSuite>
  );
}
