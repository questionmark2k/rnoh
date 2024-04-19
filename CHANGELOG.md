# Changelog

## v0.72.22
- [breaking] react-native-openharmony ships with a har file rather than module.tgz - pack-harmony and unpack-harmony should no longer be needed and they will be removed in the future
- added setting native node `id`s to `testID` or `nativeID` prop values ([6a7d0a5cc3](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/6a7d0a5cc373c7a01331c1515218f31e2ab19d9c))
- changed blocklist in metro configuration ([aba0358d36](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/aba0358d369ca9dbd3515959dfbd9e5106f9ca92))
- reverted code added in this PR - while this fixes touches, it breaks some styling in ScrollView with RefreshControl but working touches seems more important ([5872d9629a](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/5872d9629a51d7ce6ceaa565aa688222c0940ae5))
- support alignment in text measurement, calculate the correct position for children views ([6092118b76](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/6092118b769f88dfe6e1570dc1053bcc5f16d7ed))
- support setting `TextInput` component's contents by setting its `children` ([569513d344](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/569513d3444bb55cefbdbf9c419871a8cc8e0d44))


## v0.72.21
- [breaking] `onWindowSetup` is now private and was renamed to onAppSetup - use one of the following methods onPrepareRNInstance, onWindowSetup, or onWindowStageSetup instead ([98682eb3bf](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/98682eb3bf7e6284647843f599ca37cf87c94f60))
- added onKeyPress event ([e9887ef98c](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/e9887ef98cd9e3594432e2228a8c0bc0df268c6c))
- added support for `ScrollView::disableIntervalMomentum`  ([17f45cea3f](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/17f45cea3fc161b9b7be3ea16987a43c0cbec885))
- added sorting `snapToOffsets` before passing them to ArkUI node - the ArkUI snapping is not working when the offsets are not ordered  ([17f45cea3f](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/17f45cea3fc161b9b7be3ea16987a43c0cbec885))
- added passing `INFINITY` as a last snap if `snapToOffsets` has only 1 element to tackle bug #988  ([17f45cea3f](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/17f45cea3fc161b9b7be3ea16987a43c0cbec885))
- add overridable `defaultBackPressHandler()` to `RNAbility` ([35e4bf30fa](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/35e4bf30fa0e7a5670a50651c75cca787c849320))
- add `defaultBackPressHandler` to `RNInstanceCoordinator` dependencies ([35e4bf30fa](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/35e4bf30fa0e7a5670a50651c75cca787c849320))
- added `react-native verify-package-harmony` CLI command ([7d4ef1fe4a](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/7d4ef1fe4aed20708542d17f98d6af8a4735478e))
- add `ScrollView::contentOffset` prop support ([a324775f09](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/a324775f090e59f6a63b04b377ef22ec8c5be3fb))
- added missing C_API tags ([b93b924a9c](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/b93b924a9ced15f7ccc1a25fab58a8080ca7edec))
- added build-profile.json5 in react_native_openharmony folder ([996de9afb7](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/996de9afb759d6940f9712b7b98f4a08082aef04))
- added event counter to TextInput in C-API architecture. ([9ddcf52121](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/9ddcf5212144793274593dac09b76163b0f4a7c0))
- adds default issue template for bug reporting ([db2a63a5da](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/db2a63a5da1847a0d9664045871cf5caf7116f21))
- added tests for View::NextFocus and Button::NextFocus ([5f7e94cdbd](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/5f7e94cdbdd1e99580a6f9b2eab18077b199a754))
- added additional test for loading images from different subfolders. ([5dde8e1adf](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/5dde8e1adfc4a08c364e892c481257fcdb47b5dd))
- added support for setting different padding on each edge in `TextInput` ([725bf8fa08](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/725bf8fa08f80b1c26ac6f6488ad3849f720c2d6))
- added `resolveEdges` method that resolves `YogaEdges` object into `RectangleEdges<>`  ([725bf8fa08](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/725bf8fa08f80b1c26ac6f6488ad3849f720c2d6))
- added `RNInstancesCoordinator` ([de24f8e125](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/de24f8e1257438235226ce52b205f5f344916abf))
- added test for that case ([f5576361ba](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/f5576361bad8f272312fc15f4d2c9fecb87c69e0))
- add lazy bounding box calculation ([d0cf8628fd](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/d0cf8628fdc64cb1b6b89c30ae6105d43d34d068))
- fixed root `Modal's` root `Stack` node's position to `{0,0}` . ([2d1826f9ef](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/2d1826f9efb61920bdff8c448ddd841244829734))
- changed order of event firing ([e9887ef98c](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/e9887ef98cd9e3594432e2228a8c0bc0df268c6c))
- changed oh module and package name (third party libraries should import from `@rnoh/react-native-openharmony` instead of `rnoh`) ([41eff41379](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/41eff4137968b8d08bb5583efffb563d925f57f8))
- changed the `backgroundColor` of `TextInputNode` and `TextAreaNode` to `transparent` when the property is not set ([f5576361ba](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/f5576361bad8f272312fc15f4d2c9fecb87c69e0))
- removed build-profile.json5 in react_native_openharmony folder from .gitignore ([996de9afb7](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/996de9afb759d6940f9712b7b98f4a08082aef04))
- settet NODE_TEXT_INPUT_ENABLE_KEYBOARD_ON_FOCUS to True in TextInputNode constructor. ([a50d7edef8](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/a50d7edef885c0f8000f56ba60ce80a5935c60c5))
- call `defaultBackPressHandler()` in `DeviceEventManagerTurboModule.invokeDefaultBackPressHandler`  ([35e4bf30fa](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/35e4bf30fa0e7a5670a50651c75cca787c849320))
- unskip relevant tests ([a324775f09](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/a324775f090e59f6a63b04b377ef22ec8c5be3fb))
- refactor of SectionList tests ([b93b924a9c](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/b93b924a9ced15f7ccc1a25fab58a8080ca7edec))
- refined RNOHCoreContext ([de24f8e125](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/de24f8e1257438235226ce52b205f5f344916abf))
- nested scrolling is now handled manually ([dc879c4aac](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/dc879c4aac75a5a98f881a676005040d7ea79c5b))
- when a child Scroll is scrolled from its end towards the outside, the parent Scroll starts scrolling ([dc879c4aac](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/dc879c4aac75a5a98f881a676005040d7ea79c5b))
- when a child Scroll is scrolled from its end towards the inside or it is scrolled from its inside, the parent Scroll will not start scrolling when the child's end is reached ([dc879c4aac](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/dc879c4aac75a5a98f881a676005040d7ea79c5b))
- bouncing is now turned off in nested Scrolls whose content size is smaller than container size (just like on iOS) ([dc879c4aac](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/dc879c4aac75a5a98f881a676005040d7ea79c5b))
- conditional include for libqos ([233029f294](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/233029f29488fdddd2012bbea4fdb17f68551fe7))
- logging warning when calling setTaskThreadPriority on C_API_ARCH disabled ([233029f294](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/233029f29488fdddd2012bbea4fdb17f68551fe7))
- unskipped passing tests in `ModalTest.tsx` ([2d1826f9ef](https://gl.swmansion.com/rnoh/react-native-harmony/-/commit/2d1826f9efb61920bdff8c448ddd841244829734))


## v0.72.20
- added Systrace support
- fixed nativeEvent timestamps unit

### ArkTS architecture
- added support for placing images in a custom assets directory
- fixed ScrollView styles when using RefreshControl
- fixed changing TextInput value

### C-API architecture
- various touch handling improvements
- added ScrollView events support in Animated 
- added support for touches in nested Text components
- added TextInput support
- fixed ScrollView alignment
- fixed dynamically changing number of lines in a Text component
- fixed TextInput default padding and caret color
- fixed touches not being cancelled
- improved error handling

## v0.72.19
- added basic Share module support
- fixed Linking::openSettings

### ArkTS architecture
- added TextInput::contextMenuHidden support

### C-API architecture
- added RefreshControl support
- added Text specific props support
- added Modal support
- added ArkTSTurboModule::callSync method that can be used to call turbo modules from ComponentInstances
- added a way to block scrolling

## v0.72.18
### ArkTS architecture
- added LogBox support
- added Image::onLoadEnd support
- added Image::defaultSource support
- fixed Image position not being updated properly
- fixed dimensions not being set correctly when they were set to 0
- fixed crashes due to Image cache issues
- fixed displaying error dialog
- fixed dismissing alerts

### Experimental C-API architecture
- added common View props support
- added Native animations support
- added transform style prop support
- added ActivityIndicator support
- added common ScrollView props support
- added common Switch props support
- added common Image props support
- added basic backward compatibility with ArkTS arch
- changed the design of CppComponentInstance class

## v0.72.17
- added foundations for an experimental C-API based architecture
- added optional approach of building components based on LazyForEach and DataSources that provides better performance
- added `TextInput::onSelectionChange` support
- added `TextInput::selection` support
- added `TextInput::selectTextOnFocus` support
- optimized disk caching of Images
- fixed react-native codegen crash when generating code
- fixed `Switch::value`
- fixed `Image` intercepting touches when it shouldn't

## v0.72.16
- BREAKING: removed dependencies on RNAbility in RNOHContext
- added internal import resolving for harmony packages with `redirectInternalImports: true`
- added support for `ToastAndroid`
- added `enableNDKTextMeasuring` feature flag which allows optional faster text measuring using NDK
- added support for Appearance API
- restored partial support for `TextInput::onKeyPress`
- fixed `Alert` breaking when created without any buttons
- fixed websockets not connecting after updating to API 11
- fixed `fontHeight` calculations
- fixed `Modal` being shifted up by soft keyboard instead of being covered
- fixed `RefreshControl` not working correctly in nested `ScrollViews`
- fixed `Touchables` not detecting all touches when nested in `ScrollView`
- fixed `topLayout` event being unsupported

## v0.72.15

- BREAKING: deprecated `RNComponentFactory` in favour of `wrappedCustomRNComponentBuilder` 
which must now be provided to `RNApp` ([!797](https://gl.swmansion.com/rnoh/react-native-harmony/-/merge_requests/797))
- BREAKING: `RNSurface` expects `RNComponentContext` instead of `RNOHContext` ([!797](https://gl.swmansion.com/rnoh/react-native-harmony/-/merge_requests/797))
- BREAKING: asset file names in HAP resources are now hashes of the asset path instead of encoded asset paths ([!810](https://gl.swmansion.com/rnoh/react-native-harmony/-/merge_requests/810))
- added imports hijacking support for packages with no harmony native code
- added `Modal::animationType` support
- added support for loading assets from Metro dev server
- added partial `TextInput::clearButtonMode` support
- added `FileJSBundleProvider` for loading bundles from device filesystem
- fixed errors when trying to access unimplemented `UIManager` functions
- fixed issues with `TextInput` updates not applying correctly
- fixed `Text::textShadowRadius` not being applied correctly
- fixed `Image` errors not being reported correctly
- fixed `Dimensions` display metrics not updating on window size changes
- fixed `ScrolLView::scrollToEnd` not scrolling when called after `onLayout`
- fixed `TextInput::value` and `TextInput::defaultValue` no applying correctly
- fixed `RNApp` not being covered by the virtual keyboard

## v0.72.14

- added `DevSettings::addMenuItem` support
- added `TextInput::onEndEditing` support
- added Codegen support
- added `BlobTurboModule` support
- added JS debugger and profiler support
- disabled background thread by default
- fixed crashes in `Image`
- fixed scrolling issues when `ScrollView` contains a collapsable `PanResponder`
- fixed crashes connected to TextHeightAdaptivePolicy
- fixed crashes upon `RNInstance` destroying
- fixed deadlocks related to text measurements when shutting down surface
- fixed `View::pointerEvents::box-only`
- fixed `Modal` component not adapting to window size changes

## v0.72.13
- added `View::focusable` support
- added `DevLoadingView` support
- added `Text::adjustFontSizeToFit` support
- added RedBox support
- added LogBox support
- added Layout inspector support
- added `DevMenu` support
- added hot reloading support
- added `TextInput::allowFontScaling` support
- added `Image::resizeMethod` support
- removed `TextInput::onKeyPress`
- fixed errors on loading `Image`
- fixed border displaying in `Image`
- fixed nested scrolling issues
- fixed `textAlign: justify`
- fixed timers not being cleaned up when TimingTurboModule was destroyed
- fixed `Modal` not changing position after screen rotation
- fixed `TextInputState` issues
- fixed `TextInput` sending change event when value changed in props
- fixed large bundles not being able to load from Metro
- fixed issues with instance cleanup
- fixed `ScrollView` offset not updating after content size changed
- fixed native responder unlocking too soon
- fixed problems with disabled components intercepting touches
- fixed issues related to app teardown
- fixed `RefreshControl::progressViewOffset`
- fixed crashes with NativeAnimated destructor
- fixed crashes when using `PanResponder`
- fixed crashes on subsequent app starts
- fixed `Animated.View` props animations
- fixed `onPress` in nested `Text`

## v0.72.12
- added `ScrollView::inverted` prop support
- added more detailed logs
- added `ScrollView::maintainVisibleContentPosition` support
- added `TextInput::clearOnFocus` support
- added `hints` support:
  - `debug` hint
  - `ready` hint
- added `RNInstance::blockComponentsGestures` method
- added error handling for errors thrown from turbo modules
- fixed unnecessary text wrapping issue
- fixed touches when transform was provided
- fixed image sometimes not changing on source change
- fixed imports not being hijacked to scoped third-party harmony packages
- fixed `fontColor` in `TextInput`
- fixed opacity exceeding the range
- fixed some props sometimes not being provided to native components
- fixed RefreshControl behavior
- fixed inserting ellipsis when `Text` has padding
- fixed opacity not being animated when using layout animations
- fixed reacting on dimensions change on devices with folding screen
- fixed aggressive page snapping

## v0.72.11
- BREAKING: `react-native-harmony/metro.config` exports now a function that creates configuration instead of configuration
- added `TextInput::keyboardType` support
- added a way to replace implementation of any RN component
- added custom fonts support
- added animated gif support in `Image` component
- added http redirects support in `Image` component
- added `arrayBuffer` response type support in `fetch`
- added `Animated.{diffClamp, tracking, modulus}` support when native driver is enabled
- added `I18nManager::isRTL` support
- added data URIs in `Image`
- added basic `RefreshControl::progressViewOffset` support
- added `console.log` support
- added Image::alt support
- added padding support in `TextInput`
- added `ScrollView::snapToStart/End` support
- added `TextInput::onBlur` support
- added sms schema support in `Linking` module
- added `TextInput::onKeyPress` support
- added `requireNativeComponent` support
- added `RootTagContext` support
- added `ScrollView::pagingEnabled` support
- added `ScrollView::snapToAlignment` support
- added `ScrollView::flashScrollIndicators` support
- added `ScrollView::disableIntervalMomentum` support
- fixed text being slightly bigger than views containing that text
- fixed infinite `TextInput::value` updates
- fixed crashes when view inside text was trimmed by `numberOfLines`
- fixed deadlock when using multiple surfaces
- fixed touch issues caused by not unregistering component managers properly
- fixed `textDecorationLine` color
- fixed `ScrollView` position when content size changes
- fixed freeze caused by having multiple empty lines in `Text` component
- fixed font style not being applied to `TextInput::placeholder`
- fixed jittering sticky headers
- fixed precision of layout positions and dimensions
- fixed jumping animations, when a render happens during native animation
- fixed vertical alignment of text and attachments inside `Text`
- fixed displaying multiple attachments on top of each other in `Text`
- fixed padding in `Text` component
- fixed WebSocket error handling
- fixed `VirtualizedList::initialIndex`
- fixed `TextInput` autofocus outside of modal
- fixed `PlatformConstants::reactNativeVersion` format

## v0.72.10
- breaking change: SurfaceHandle::start is now async
- added basic support for KeyboardAvoidingView
- added Model to Platform.constants
- added Image::onLoad support
- added basic TextInput::autoFocus support
- fixed loading remote images when urls is redirected
- fixed presses for views with translations animated with native driver
- fixed text wrapping after characters
- fixed onBlur crashing the app
- fixed empty values in TextInputs
- fixed maxLength in TextInput
- fixed nested scroll views
- fixed RNViewManager::parentTag incorrectly returning undefined
- fixed Text's onLayout
- fixed deadlock caused by creating/updating/destroying surfaces
- fixed unnecessary text wrapping
- fixed locking scroll from component manager

## v0.72.9
- breaking change: SurfaceHandle::stop and SurfaceHandle::destroy are now async
- added hitSlop support
- added basic onBlur support
- added support for TextInput content passed in onFocus event
- added basic useColorScheme support
- added Animated.decay support
- added Animated.spring support
- added ScrollView::scrollToEnd support
- fixed incorrect lineHeight calculation
- fixed onPress not firing when view and pressable were flattened
- fixed onPress working unreliably
- fixed TextInput autofocus
- fixed TextInput::value
- fixed wrapping of Chinese characters
- fixed single view in Text crashing the app
- fixed not working props for multiline TextInput
- fixed Animated.Value callback timing issue
- fixed deadlock caused when creating/destroying surfaces

## v0.72.8
- added layout animations support
- added ellipsis support when text components are nested or text component inside another text component
- fixed timer behavior when an app is in background
- fixed text input styling issues
- fixed `fetch`
- fixed errors reported by new ArkTS linter
- fixed app crashes caused sometimes caused by touches

## v0.72.7
- added ScrollView::persistentScrollbar
- added ActivityIndicator support
- added Text props/styles support:
  - vertical alignement
  - letter spacing 
  - selectable
  - ellipsizeMode
  - disabled
  - textShadow
- added View::borderStyle
- added ScrollView::contentOffset prop support
- added edge specific border width support
- added overflow support
- added ScrollView::scrollEnabled support
- added ScrollView::indicatorStyle support
- added ScrollView::decelerationRate support
- added basic linking API support
- added an option to blur images
- added nested text support
- added views inside text support
- added hermes bytecode bundles support
- added basic Keyboard module support
- added SafeAreaView support
- support TextInput props:
  - editable
  - caretHidden
  - maxLength
  - selectionColor
  - secureTextEntry
  - placeholder
  - placeholderTextColor
- fixed custom RefreshControl components
- fixed timers not being paused when the app is in background
- fixed awaiting JS Bundle execution
- updated React Native to 0.72.5
- changed approach to handling touches
  
## v0.72.6
- added support for Switch:disabled
- added support for Animated.Value listeners
- added Image.getSize support
- added Image::resizeMode prop support
- added RefreshControl component support
- added Text::textDecoration and Text::textDecorationColor support
- added iOS View shadow props support
- added support for multiple RN Instances and surfaces
- added border edge specific colors
- added interface JSBundleProvider
- added Image::tintColor support
- added ComponentManagers
- added Image::prefetch support
- added Text::padding support
- added View::borderStyle support
- fixed DrawerLayoutAndroid causing compilation errors
- fixed flickering modal contents
- fixed bundle-harmony arguments
- changed assets directory
- fixed deadlock caused by timing and animated turbo modules

## v0.72.5
- added support for Switch component
- reexported DeviceEventEmitter and findNodeHandle
- improved logging fatal errors
- fixed ScrollView offset when a ScrollView had a border
- fixed tap not stopping scrolling

## v0.72.4
- BREAKING CHANGE: replaced `EventEmitterRegistry.cpp` with `ShadowViewRegistry.cpp`
- added `Modal` component support
- added an option to set CPP state from eTS
- added Addition, Subtraction, Multiplication, and Division Animation Nodes
- fixed transforms
- reexported `dispatchCommand` method for custom fabric components that don't use code generation utility

## v0.72.3
- fixed TextInput not working correctly since v0.72.0
- fixed crashing on Open Harmony 4.0.8.5

## v0.72.2
- BREAKING CHANGE: renamed harmony module from `@ohos/rnoh` to `rnoh`
- BREAKING CHANGE: changed the interface of `EventEmitRequestHandler` and `Package::createEventEmitRequestHandlers`
- added support for `pointerEvents`
- added support for scroll driven animations to  native driver
- added support for horizontal scrolling
- added support for `BackHandler.exitApp()`
- various memory management tweaks that probably fix some memory leaks
- bumped react-native-harmony-cli to fix crash caused by `node_modules/.bin` when unpacking harmony modules

## v0.72.1
- fixed react-native unpack-harmony not replacing the native module when updating npm package
- fixed crash when removing a scrolling ScrollView
- fixed color transformation for custom components when a NapiBinder was not provided
- fixed an error thrown by React Native renderer caused by dispatching wrong touch events 
- fixed `collapsable` property
- fixed `onLayout` property
- fixed `FlatList` displaying wrong items
- added development mode (which can be disabled by using `prod` flag: `react-native bundle-harmony --prod`)
- added caching text measurements
- added a basic support for a native driver
- added back button handler - requires adapting `entry/pages/Index.ets` code, since listener for back button callbacks can only be added to the entry component
- removed rnInstance property from RNOHContext in favor of rnInstanceManager
- 
## v0.72.0
- upgraded React Native from 0.71.3 to 0.72.0
- added support for transform properties
- added support for momentum scroll events
- improved setTimeout by respecting native call delay
- fixed layout issues after hiding status bar

## v0.0.20
- added RNBaseView in order to share common styles and handlers with custom components
- added ability to provide custom logger
- added text width measurer (height must be provided)
- added support for nesting Text components
- added support for specifying border radius per corner
- added support for text alignement
- added ability to overwrite bundle loading code
- added basic support for StatusBar
- improved rendering performance
- styles support should be similar across various components
- fixed drawing order
- fixed touches in ScrollView

## v0.0.19
- breaking change: changed interface on the native side
  - made the `RNAbility` more robust
- improved lifecycle management
- fixed background/border colors updates
- added an option to send initialProps from native side to RN app

## v0.0.18
- breaking change: removed `@Provide`/`@Consume` from RNOH components to improve performance
- fixed source of fatal crashes
- fixed default colors
- added a event communication channel for third party libraries
- added support for AppState
