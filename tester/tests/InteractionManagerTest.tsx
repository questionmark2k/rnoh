import {InteractionManager} from 'react-native';
import {TestSuite} from '@rnoh/testerino';
import {TestCase} from '../components';

export function InteractionManagerTest() {
  return (
    <TestSuite name="InteractionManager">
      <TestCase.Logical
        itShould="execute callback to be executed without crashing"
        fn={async () => {
          return new Promise(resolve => {
            InteractionManager.runAfterInteractions(resolve);
          });
        }}
      />
    </TestSuite>
  );
}
