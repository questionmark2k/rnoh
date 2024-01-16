import {TestCase, TestSuite} from '@rnoh/testerino';
import {
  SampleTurboModule,
  GeneratedSampleTurboModule,
} from 'react-native-harmony-sample-package';

export function TurboModuleTest() {
  return (
    <TestSuite name="TurboModule">
      <TestSuite name="manual implementation">
        <TestCase
          itShould="return null when calling getNull()"
          fn={({expect}) => {
            expect(SampleTurboModule.getNull(null)).to.be.null;
          }}
        />
        <TestCase
          itShould="return [1, 2, 3] when calling getArray"
          fn={({expect}) => {
            expect(SampleTurboModule.getArray([1, 2, 3])).to.eql([1, 2, 3]);
          }}
        />
        <GeneratedTurboModuleTestCases sampleTurboModule={SampleTurboModule} />
      </TestSuite>
      <TestSuite name="generated">
        <GeneratedTurboModuleTestCases
          sampleTurboModule={GeneratedSampleTurboModule}
        />
      </TestSuite>
    </TestSuite>
  );
}

function GeneratedTurboModuleTestCases({
  sampleTurboModule,
}: {
  sampleTurboModule: typeof GeneratedSampleTurboModule;
}) {
  return (
    <>
      <TestCase
        itShould="return null when calling voidFunc()"
        fn={({expect}) => {
          expect(sampleTurboModule.voidFunc()).to.be.null;
        }}
      />
      <TestCase
        itShould="return true when calling getBool(true)"
        fn={({expect}) => {
          expect(sampleTurboModule.getBool(true)).to.be.true;
        }}
      />

      <TestCase
        itShould="return { x: { y: 1 } } when calling getObject"
        fn={({expect}) => {
          expect(sampleTurboModule.getObject({x: {y: 1}})).to.eql({
            x: {y: 1},
          });
        }}
      />
      <TestCase
        itShould="call the callback and providing string argument"
        fn={async ({expect}) => {
          const promise = new Promise<string>(resolve => {
            sampleTurboModule.registerFunction(resolve);
          });
          expect(typeof (await promise)).to.be.eq('string');
        }}
      />
      <TestCase
        itShould="handle async jobs"
        fn={async ({expect}) => {
          const result = await sampleTurboModule?.doAsyncJob(true);
          expect(typeof result).to.be.eq('string');
        }}
      />
      <TestCase
        itShould="handle errors in async jobs"
        fn={async ({expect}) => {
          let errMsg: string | undefined;
          try {
            await sampleTurboModule?.doAsyncJob(false);
          } catch (err) {
            errMsg = (err as Error).message;
          }

          expect(errMsg).to.be.eq('rejected on native side');
        }}
      />
      <TestCase
        itShould="get an array asynchronously"
        fn={async ({expect}) => {
          expect(await sampleTurboModule.getPromisedArray()).to.be.eql([
            1, 2, 3,
          ]);
        }}
      />
    </>
  );
}
