import {
  FlatList,
  RefreshControl,
  ScrollView,
  StyleSheet,
  Text,
  View,
} from 'react-native';
import {TestSuite} from '@rnoh/testerino';
import {useEffect, useState} from 'react';
import {TestCase} from '../components';

export const RefreshControlTest = () => {
  const [refreshKey, setRefreshKey] = useState(0);

  useEffect(() => {
    setInterval(() => setRefreshKey(prev => prev + 1), 1000);
  }, []);

  return (
    <TestSuite name="RefreshControl">
      <TestCase.Example
        tags={['C_API']}
        itShould="display refresh control every second">
        <ScrollView
          style={{height: 128, backgroundColor: 'white'}}
          refreshControl={
            <RefreshControl
              refreshing={refreshKey % 2 === 0}
              onRefresh={() => {}}
            />
          }
        />
      </TestCase.Example>
      <TestCase.Example
        tags={['C_API']}
        itShould="display refresh control with tintColor">
        <ScrollView
          style={{height: 128, backgroundColor: 'white'}}
          refreshControl={
            <RefreshControl
              refreshing={refreshKey % 2 === 0}
              tintColor={'#FFC0CB'}
              onRefresh={() => {}}
            />
          }
        />
      </TestCase.Example>
      <TestCase.Example
        modal
        tags={['C_API']}
        itShould="be refreshing for one second after pull to refresh">
        <PullToRefreshExample />
      </TestCase.Example>
      <TestCase.Example
        modal
        tags={['C_API']}
        itShould="immediately stop refreshing after pulling to refresh">
        <PullToRefreshExample doNothingOnRefresh />
      </TestCase.Example>
      <TestCase.Example
        modal
        tags={['C_API']}
        itShould="refresh with progressViewOffset = undefined"
        skip={{
          android: false,
          harmony:
            'there is a restriction on how tall the progress view can be - should be removed in API 11',
        }} // https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/624
      >
        <PullToRefreshProgressViewOffset />
      </TestCase.Example>
      <TestCase.Example
        modal
        tags={['C_API']}
        itShould="refresh with progressViewOffset = 50"
        skip={{
          android: false,
          harmony:
            'there is a restriction on how tall the progress view can be - should be removed in API 11',
        }} // https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/624
      >
        <PullToRefreshProgressViewOffset progressViewOffset={50} />
      </TestCase.Example>
      <TestCase.Example
        modal
        tags={['C_API']}
        itShould="refresh with progressViewOffset = 100"
        skip={{
          android: false,
          harmony:
            'there is a restriction on how tall the progress view can be - should be removed in API 11',
        }} // https://gl.swmansion.com/rnoh/react-native-harmony/-/issues/624
      >
        <PullToRefreshProgressViewOffset progressViewOffset={100} />
      </TestCase.Example>
      <TestCase.Example
        tags={['C_API']}
        itShould="Refresh control in nested scroll view - one source of truth for both RefreshControl (one state)"
        modal>
        <PullToRefreshInNestedScrollViews />
      </TestCase.Example>
      <TestCase.Example
        tags={['C_API']}
        itShould="Refresh control in nested scroll view - two sources of truth - one for each RefreshControl (two states)"
        modal>
        <PullToRefreshInNestedScrollViewsDifferentSource />
      </TestCase.Example>
      <TestCase.Example
        tags={['C_API']}
        itShould="Refresh control in nested scroll view - two sources of truth - one for each RefreshControl (two states) - with the content between"
        modal>
        <PullToRefreshInNestedScrollViewsDifferentSourceContentBetween />
      </TestCase.Example>
    </TestSuite>
  );
};

function PullToRefreshExample({
  doNothingOnRefresh,
}: {
  doNothingOnRefresh?: boolean;
}) {
  const [isRefreshing, setIsRefreshing] = useState(false);

  return (
    <FlatList
      style={{height: 256}}
      refreshing={isRefreshing}
      onRefresh={() => {
        if (!doNothingOnRefresh) {
          setIsRefreshing(true);
          setTimeout(() => setIsRefreshing(false), 1000);
        }
      }}
      data={[1, 2, 3, 4, 5]}
      renderItem={({item}) => (
        <Text style={{height: 96, borderBottomWidth: 1}}>{item}</Text>
      )}
    />
  );
}

function PullToRefreshProgressViewOffset({
  progressViewOffset,
}: {
  progressViewOffset?: number;
}) {
  const [refreshing, setIsRefreshing] = useState(false);

  return (
    <ScrollView
      style={{height: '90%', backgroundColor: 'lightgray'}}
      refreshControl={
        <RefreshControl
          refreshing={refreshing}
          onRefresh={() => {
            setIsRefreshing(true);
            setTimeout(() => setIsRefreshing(false), 3000);
          }}
          progressViewOffset={progressViewOffset}
        />
      }>
      <View style={{height: 50, backgroundColor: 'lightblue'}}>
        <Text>First Content Component</Text>
      </View>
      <View style={{height: 50, backgroundColor: 'lightgreen'}}>
        <Text>Second Content Component</Text>
      </View>
      <View style={{height: 50, backgroundColor: 'lightblue'}}>
        <Text>Third Content Component</Text>
      </View>
    </ScrollView>
  );
}

const wait = (timeout: number) => {
  return new Promise(resolve => {
    setTimeout(resolve, timeout);
  });
};

const PullToRefreshInNestedScrollViews = () => {
  const [refreshing, setRefrehing] = useState(false);

  const onRefresh = () => {
    setRefrehing(true);
    wait(2000).then(() => {
      setRefrehing(false);
    });
  };

  return (
    <ScrollView
      style={{width: '100%'}}
      contentContainerStyle={[styles.scrollView]}
      refreshControl={
        <RefreshControl refreshing={refreshing} onRefresh={onRefresh} />
      }>
      <View
        style={{
          backgroundColor: 'lightblue',
          width: '100%',
          height: 250,
        }}>
        {Array.from({length: 5}).map((_, index) => (
          <Text
            key={index}
            style={{textAlign: 'center', height: 50, borderWidth: 1}}>
            {`Parent ScrollView Item ${index + 1}`}
          </Text>
        ))}
      </View>
      <ScrollView
        style={{width: '100%', height: 248}}
        contentContainerStyle={{
          width: '100%',
        }}
        refreshControl={
          <RefreshControl refreshing={refreshing} onRefresh={onRefresh} />
        }>
        <View
          style={{
            backgroundColor: 'pink',
            width: '100%',
          }}>
          {Array.from({length: 15}).map((_, index) => (
            <Text
              key={index}
              style={{textAlign: 'center', height: 50, borderWidth: 1}}>
              {`Child ScrollView Item ${index + 1}`}
            </Text>
          ))}
        </View>
      </ScrollView>
    </ScrollView>
  );
};

const PullToRefreshInNestedScrollViewsDifferentSource = () => {
  const [isRefreshingOne, setIsRefreshingOne] = useState(false);
  const [isRefreshingTwo, setIsRefreshingTwo] = useState(false);

  const onFirstRefresh = () => {
    setIsRefreshingOne(true);
    wait(2000).then(() => {
      setIsRefreshingOne(false);
    });
  };

  const onSecondRefresh = () => {
    setIsRefreshingTwo(true);
    wait(5000).then(() => {
      setIsRefreshingTwo(false);
    });
  };

  return (
    <View>
      <ScrollView
        style={{width: '100%'}}
        contentContainerStyle={[styles.scrollView]}
        refreshControl={
          <RefreshControl
            refreshing={isRefreshingOne}
            onRefresh={onFirstRefresh}
          />
        }>
        <View
          style={{
            backgroundColor: 'lightblue',
            width: '100%',
            height: 250,
          }}>
          {Array.from({length: 5}).map((_, index) => (
            <Text
              key={index}
              style={{textAlign: 'center', height: 50, borderWidth: 1}}>
              {`Parent ScrollView Item ${index + 1}`}
            </Text>
          ))}
        </View>
        <ScrollView
          style={{width: '100%', height: 248}}
          contentContainerStyle={{
            width: '100%',
          }}
          refreshControl={
            <RefreshControl
              refreshing={isRefreshingTwo}
              onRefresh={onSecondRefresh}
            />
          }>
          <View
            style={{
              backgroundColor: 'pink',
              width: '100%',
            }}>
            {Array.from({length: 15}).map((_, index) => (
              <Text
                key={index}
                style={{textAlign: 'center', height: 50, borderWidth: 1}}>
                {`Child ScrollView Item ${index + 1}`}
              </Text>
            ))}
          </View>
        </ScrollView>
      </ScrollView>
    </View>
  );
};

const PullToRefreshInNestedScrollViewsDifferentSourceContentBetween = () => {
  const [isRefreshingOne, setIsRefreshingOne] = useState(false);
  const [isRefreshingTwo, setIsRefreshingTwo] = useState(false);

  const onFirstRefresh = () => {
    setIsRefreshingOne(true);
    wait(2000).then(() => {
      setIsRefreshingOne(false);
    });
  };

  const onSecondRefresh = () => {
    setIsRefreshingTwo(true);
    wait(5000).then(() => {
      setIsRefreshingTwo(false);
    });
  };

  return (
    <View>
      <ScrollView
        style={{width: '100%'}}
        contentContainerStyle={[styles.scrollView]}
        refreshControl={
          <RefreshControl
            refreshing={isRefreshingOne}
            onRefresh={onFirstRefresh}
          />
        }>
        <View
          style={{
            backgroundColor: 'lightblue',
            width: '100%',
            height: 250,
          }}>
          {Array.from({length: 5}).map((_, index) => (
            <Text
              key={index}
              style={{textAlign: 'center', height: 50, borderWidth: 1}}>
              {`Parent ScrollView Item ${index + 1}`}
            </Text>
          ))}
        </View>
        <View
          style={{
            backgroundColor: 'red',
            width: '100%',
            height: 250,
            justifyContent: 'center',
          }}>
          <Text style={{textAlign: 'center'}}>Some other content</Text>
        </View>
        <ScrollView
          style={{width: '100%', height: 248}}
          contentContainerStyle={{
            width: '100%',
          }}
          refreshControl={
            <RefreshControl
              refreshing={isRefreshingTwo}
              onRefresh={onSecondRefresh}
            />
          }>
          <View
            style={{
              backgroundColor: 'pink',
              width: '100%',
            }}>
            {Array.from({length: 15}).map((_, index) => (
              <Text
                key={index}
                style={{textAlign: 'center', height: 50, borderWidth: 1}}>
                {`Child ScrollView Item ${index + 1}`}
              </Text>
            ))}
          </View>
        </ScrollView>
        <View
          style={{
            backgroundColor: 'red',
            width: '100%',
            height: 250,
            justifyContent: 'center',
          }}>
          <Text style={{textAlign: 'center'}}>Some other content</Text>
        </View>
        <View
          style={{
            backgroundColor: 'blue',
            width: '100%',
            height: 250,
            justifyContent: 'center',
          }}>
          <Text style={{textAlign: 'center'}}>Some other content</Text>
        </View>
      </ScrollView>
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
  },
  scrollView: {
    flex: 1,
    width: '100%',
    alignItems: 'center',
    justifyContent: 'center',
  },
  text: {
    backgroundColor: 'green',
  },
});
