import {TestSuite} from '@rnoh/testerino';
import {Button} from 'react-native';
import {TestCase} from '../components';

const SEVERITIES = ['log', 'info', 'warn', 'error', 'debug'] as const;

export const ConsoleTest = () => {
  return (
    <TestSuite name="Console">
      {SEVERITIES.map((severity, idx) => (
        <TestCase.Example
          key={idx}
          itShould={`log message to console (${severity})`}>
          <Button
            title="log"
            onPress={() => console[severity](`console.${severity} test`)}
          />
        </TestCase.Example>
      ))}
    </TestSuite>
  );
};
