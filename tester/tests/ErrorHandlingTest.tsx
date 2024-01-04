import {TestCase, TestSuite} from '@rnoh/testerino';
import {Button} from '../components';
import {SampleTurboModule} from './__fixtures__';

export function ErrorHandlingTest() {
  return (
    <TestSuite name="ErrorHandling">
      <ErrorBoxTestCase
        itShould="display error box with 'ERROR_MESSAGE' on the red background and a native stack of that error"
        errorData={{whatHappened: 'ERROR_MESSAGE', howCanItBeFixed: []}}
      />
      <ErrorBoxTestCase
        itShould="display one suggestion"
        errorData={{
          whatHappened: 'ERROR_MESSAGE',
          howCanItBeFixed: ['Press "Dismiss"'],
        }}
      />
      <ErrorBoxTestCase
        itShould="display two suggestions"
        errorData={{
          whatHappened: 'ERROR_MESSAGE',
          howCanItBeFixed: ['Press "Dismiss"', 'Press "Reload'],
        }}
      />
      <ErrorBoxTestCase
        itShould="display custom stack (foo/bar/baz) instead of a native stack"
        errorData={{
          whatHappened: 'ERROR_MESSAGE',
          howCanItBeFixed: [],
          customStack: 'foo\nbar\nbaz',
        }}
      />
      <ErrorBoxTestCase
        itShould="display object extra data as JSON"
        errorData={{
          whatHappened: 'ERROR_MESSAGE',
          howCanItBeFixed: [],
          extraData: {foo: 'bar'},
        }}
      />
    </TestSuite>
  );
}

function ErrorBoxTestCase({
  itShould,
  errorData,
}: {
  itShould: string;
  errorData: {
    whatHappened: string;
    howCanItBeFixed: string[];
    extraData?: any;
    customStack?: string;
  };
}) {
  return (
    <TestCase itShould={itShould}>
      <Button
        label="Show Error Box"
        onPress={() => {
          SampleTurboModule.displayRNOHError(errorData);
        }}
      />
    </TestCase>
  );
}
