import React from 'react';
import {TestSuite} from '@rnoh/testerino';
import {TestCase} from '../components';
import SampleTurboModule2 from 'react-native-harmony-sample-package/src/NativeGeneratedSampleTurboModule2';

export function InternalTest() {
  return (
    <TestSuite name="Internal Tests">
      <TestCase.Logical
        tags={['C_API']}
        itShould="pass the same value as the one provided (ArkTS -> C++ -> ArkTS communication test)"
        fn={async ({expect}) => {
          const result = await SampleTurboModule2.emitEventFromArkTS2Cpp({
            foo: 'bar',
          });
          expect(result).to.be.eql({foo: 'bar'});
        }}
      />
    </TestSuite>
  );
}
