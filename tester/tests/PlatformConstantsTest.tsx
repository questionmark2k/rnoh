import {Platform} from 'react-native';
import {TestSuite} from '@rnoh/testerino';
import {TestCase} from '../components';

export function PlatformConstantsTest() {
  return (
    <TestSuite name="PlatformConstants">
      <TestCase.Logical
        tags={['C_API']}
        itShould="use 'harmony' as platform name"
        fn={({expect}) => {
          expect(Platform.OS).to.be.eq('harmony');
        }}
      />
      <TestCase.Logical
        tags={['C_API']}
        itShould="specify platform version"
        fn={({expect}) => {
          expect(Platform.Version.toString().split('.').length - 1).to.be.eq(3);
        }}
      />
      <TestCase.Logical
        tags={['C_API']}
        itShould="not be running in tv mode"
        fn={({expect}) => {
          expect(Platform.isTV).to.be.false;
        }}
      />
      <TestCase.Logical
        tags={['C_API']}
        itShould="select Platform properly"
        fn={({expect}) => {
          expect(
            Platform.select({
              android: 'a',
              ios: 'i',
              native: 'n',
              harmony: 'h',
            }),
          ).to.be.eq('h');
        }}
      />
      <TestCase.Logical
        tags={['C_API']}
        itShould="provide some RN version"
        fn={({expect}) => {
          expect(Platform.constants.reactNativeVersion).to.be.not.undefined;
          expect(Platform.constants.reactNativeVersion.major).to.be.not
            .undefined;
          expect(Platform.constants.reactNativeVersion.minor).to.be.not
            .undefined;
          expect(Platform.constants.reactNativeVersion.patch).to.be.not
            .undefined;
        }}
      />
      <TestCase.Logical
        tags={['C_API']}
        itShould="provide some value for isTesting"
        fn={({expect}) => {
          expect(typeof Platform.constants.isTesting).to.be.eq('boolean');
        }}
      />
      <TestCase.Logical
        tags={['C_API']}
        itShould="specify product model"
        fn={({expect}) => {
          if (Platform.OS === 'harmony') {
            expect(['NOH', 'ALN-AL00']).to.include(Platform.constants.Model);
          }
        }}
      />
    </TestSuite>
  );
}
