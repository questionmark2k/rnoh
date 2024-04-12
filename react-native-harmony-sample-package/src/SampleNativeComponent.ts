import {
  NativeSyntheticEvent,
  ProcessedColorValue,
  requireNativeComponent,
  ViewProps,
} from "react-native";

/**
 * Importing custom native components:
 * NOTE: Those functions need to be in their own files, because hot reloading can trigger them more than once..
 *
 * 1) SampleView JSI binder on CPP side needs to be provided to make this function work.
 */
export const SampleNativeComponent = requireNativeComponent<
  ViewProps & {
    size: number;
    textColor: ProcessedColorValue;
    onSampleClick?: (event: NativeSyntheticEvent<{}>) => void;
  }
>("SampleView");

/**
 * 2) An alternative to JSI binders and `requireNativeComponent` is `registerViewConfig` function (Harmony-only API).
 *
    const SampleView = registerViewConfig('SampleView', () => {
      return {
        uiViewClassName: 'SampleView',
        bubblingEventTypes: {},
        directEventTypes: {},
        validAttributes: {
          ...ReactNativeViewAttributes.UIView,
          size: true,
        },
      };
    });
 */

/**
 * 3) Once code generation is supported, `codegenNativeComponent` will be recommended.
 */
