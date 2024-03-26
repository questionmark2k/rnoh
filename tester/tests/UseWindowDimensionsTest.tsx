import {useWindowDimensions} from 'react-native';
import {TestSuite} from '@rnoh/testerino';
import {TestCase} from '../components';

export function UseWindowDimensionsTest() {
  const {height, width, scale, fontScale} = useWindowDimensions();

  return (
    <TestSuite name="useWindowDimensions">
      <TestCase.Logical
        itShould="return plausible width (greater than 0)"
        fn={({expect}) => {
          expect(width).to.be.greaterThan(0);
        }}
      />
      <TestCase.Logical
        itShould="return plausible height (greater than 0)"
        fn={({expect}) => {
          expect(height).to.be.greaterThan(0);
        }}
      />
      <TestCase.Logical
        itShould="return plausible scale (greater than 0)"
        fn={({expect}) => {
          expect(scale).to.be.greaterThan(0);
        }}
      />
      <TestCase.Logical
        itShould="return plausible fontScale (greater than 0)"
        fn={({expect}) => {
          expect(fontScale).to.be.greaterThan(0);
        }}
      />
    </TestSuite>
  );
}
