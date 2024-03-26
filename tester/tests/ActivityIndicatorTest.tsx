import {TestSuite} from '@rnoh/testerino';
import {useEffect, useState} from 'react';
import {ActivityIndicator} from 'react-native';
import {TestCase} from '../components';

// https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/290
export function ActivityIndicatorTest() {
  return (
    <TestSuite name="ActivityIndicator">
      <TestCase.Example itShould="display small, gray and animated loading indicator">
        <ActivityIndicator />
      </TestCase.Example>
      <TestCase.Example itShould="enable and disable loading every 500 ms">
        <Interval
          ms={500}
          render={refreshKey => {
            return <ActivityIndicator animating={refreshKey % 2 === 0} />;
          }}
        />
      </TestCase.Example>
      <TestCase.Example
        tags={['C_API']}
        itShould="display red loading indicator">
        <ActivityIndicator color={'red'} />
      </TestCase.Example>
      <TestCase.Example itShould="display large loading indicator">
        <ActivityIndicator size="large" />
      </TestCase.Example>
    </TestSuite>
  );
}

function Interval({
  render,
  ms,
}: {
  ms: number;
  render: (refreshKey: number) => any;
}) {
  const [refreshKey, setRefreshKey] = useState(0);

  useEffect(() => {
    const interval = setInterval(() => {
      setRefreshKey(prev => prev + 1);
    }, ms);
    return () => {
      clearInterval(interval);
    };
  }, [ms]);

  return render(refreshKey);
}
