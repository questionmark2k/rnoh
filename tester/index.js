// @ts-check
import {AppRegistry} from 'react-native';
import App from './App';
import {name as appName} from './app.json';
import {AppParamsContext} from './contexts';
// @ts-expect-error
import ReactNativeFeatureFlags from 'react-native/Libraries/ReactNative/ReactNativeFeatureFlags';
import {
  AnimationsExample,
  CheckerboardExample,
  ChessboardExample,
  CursorExample,
  FlatListVsScrollViewExample,
  ImageGalleryExample,
  LargeImageScrollExample,
  StickyHeadersExample,
  TesterExample,
  TextScrollExample,
  TogglingComponentExample,
} from './examples';
import { TextTestsExample } from './examples/TestTextsExample';

AppRegistry.setWrapperComponentProvider(appParams => {
  return ({children, ..._otherProps}) => (
    <AppParamsContext.Provider value={appParams}>
      {children}
    </AppParamsContext.Provider>
  );
});

ReactNativeFeatureFlags.shouldEmitW3CPointerEvents = () => true;
ReactNativeFeatureFlags.shouldPressibilityUseW3CPointerEventsForHover = () =>
  true;

AppRegistry.registerComponent(appName, () => App);

AppRegistry.registerComponent('tester', () => TesterExample);
AppRegistry.registerComponent('animations', () => AnimationsExample);
AppRegistry.registerComponent('text_tests', () => TextTestsExample);
AppRegistry.registerComponent('checkerboard', () => CheckerboardExample);
AppRegistry.registerComponent('chessboard', () => ChessboardExample);
AppRegistry.registerComponent('cursor', () => CursorExample);
AppRegistry.registerComponent('image_gallery', () => ImageGalleryExample);
AppRegistry.registerComponent(
  'large_image_scroll',
  () => LargeImageScrollExample,
);
AppRegistry.registerComponent('text_scroll', () => TextScrollExample);
AppRegistry.registerComponent('flat_list', () => FlatListVsScrollViewExample);
AppRegistry.registerComponent('toggling', () => TogglingComponentExample);
AppRegistry.registerComponent('sticky_headers', () => StickyHeadersExample);
