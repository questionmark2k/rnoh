# Codegen

## Introduction
Codegen is responsible for generating CPP and native code based from project's Flow and TypeScript files. It is also responsible for generating files that required to auto link packages.

## Glossary
- RN - React Native
- CODEGEN - RN feature that generates "glue" code between APP_JS_CODE and APP_NATIVE_CODE.
- APP_PROJECT - An application project, which may use PACKAGEs.
- APP_JS_CODE - Part of the APP_PROJECT written in JavaScript or TypeScript.
- APP_NATIVE_CODE - Part of the APP_PROJECT on the platform side.
- PACKAGE - An npm package used by a APP_PROJECT.

## Actors
- APP_DEVELOPER
- PACKAGE_DEVELOPER

## Design
CODEGEN will be provided by the `react-native-harmony-cli` package. The code should be written in such way that a potential extraction to `react-native-harmony-codegen` package should be easy. `@react-native/codegen` will be used to extract information from an APP_PROJECT. Due to lack of the eTS and CPP AST NPM packages, template-based approach is preferable. CODEGEN will leverage NodeJS, because it is already used by `react-native-harmony-cli`, and NodeJS can be run on MacOS, Windows and Linux.

## Testing Strategy
`SampleTurboModule` and `SampleFabricComponent` may need to be extracted from tester app to a separate package - `sample-package`, which could be used as a fixture in E2E tests.

## Research Conclusions (RN 0.73)

- CODEGEN is an experimental feature.
- CODEGEN is executed at the build time, but it can be triggered manually.
- APP_PROJECT developers don't need to be aware of CODEGEN.
- PACKAGE developers need to be aware of CODEGEN.
- CODEGEN searches for files with a specific naming convention.
- CODEGEN's results are quite different on Android and iOS.
- CODEGEN reads data from APP_PROJECT to `schema.json` which is later used to generate code.
- CODEGEN code is located in `@react-native/codegen`
- `@react-native/codegen` uses template-based code generation (instead of AST-based code generation)


| Type                          | Android                                                                                                  | iOS                                                                                                          | Harmony (suggestion)                                                         |
| ----------------------------- | -------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------- |
| CODEGEN manual execution      | `cd <APP_PROJECT_ROOT>/android && gradlew generateCodegenArtifactsFromSchema`                            | `<APP_PROJECT_ROOT>/node_modules/react-native/scripts/generate-codegen-artifacts.js`                         | `react-native codegen-harmony`                                               |
| PACKAGEs generated output dir | `<APP_PROJECT_ROOT>/node_modules/<PACKAGE>/android/build/generated/source/codegen`                       | `<USER_SPECIFIED_OUTPUT>/build/generated/ios`                                                                | `<APP_PROJECT_ROOT>/harmony/entry/src/main/ets/codegen`                      |
| Package linking file          | `<APP_PROJECT_ROOT>/android/app/build/generated/rncli/src/main/java/com/facebook/react/PackageList.java` | `<APP_PROJECT_ROOT>/ios/Pods/Headers/Private/React-RCTFabric/React/RCTThirdPartyFabricComponentsProvider.h`? | `<APP_PROJECT_ROOT>/harmony/entry/src/main/ets/codegen/RNPackagesFactory.ts` |

