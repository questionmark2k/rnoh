import {NativeEventEmitter} from 'react-native';
import {TestSuite} from '@rnoh/testerino';
import {TestCase} from '../components';

export const NativeEventEmitterTest = () => {
  return (
    <TestSuite name="NativeEventEmitter">
      <TestCase.Logical
        itShould="emit and react to events"
        fn={async ({expect}) => {
          const eventEmitter = new NativeEventEmitter();
          return new Promise(resolve => {
            eventEmitter.addListener('foo', event => {
              expect(event).to.be.eq('payload');
              resolve();
            });
            eventEmitter.emit('foo', 'payload');
          });
        }}
      />
    </TestSuite>
  );
};
