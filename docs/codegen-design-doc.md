# Codegen

## Introduction
Codegen is responsible for generating CPP and native code based from project's Flow and TypeScript files. It is also responsible for generating files that are required to auto link packages.

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

## User Stories
- As an APP_DEVELOPER, I don't want to think about code generation, so that I can focus on my application code.
- As an APP_DEVELOPER, I want to generate code for my turbo modules and fabric components, so that I don't need to touch CPP.
- As a PACKAGE_DEVELOPER, I want to partially generate code, so that I can provide my CPP turbo modules.

## Design
CODEGEN will be provided by the `react-native-harmony-cli` package. The code should be written in such a way that a potential extraction to `react-native-harmony-codegen` package should be easy. `@react-native/codegen` will be used to extract information from an APP_PROJECT. Due to lack of the eTS and CPP AST NPM packages, the template-based approach is preferable. CODEGEN will leverage NodeJS, because it is already used by `react-native-harmony-cli`, and NodeJS can be run on MacOS, Windows and Linux.

## Testing Strategy
`SampleTurboModule` and `SampleFabricComponent` may need to be extracted from tester app to a separate package - `sample-package`, which can be used as a fixture in E2E tests.

## Research 

### RN 0.73

- CODEGEN is an experimental feature.
- CODEGEN is executed at the build time, but it can be triggered manually.
- APP_PROJECT developers don't need to be aware of CODEGEN.
- PACKAGE developers need to be aware of CODEGEN.
- CODEGEN searches for files with a specific naming convention, if a PACKAGE provides "codegenConfig" in `package.json`
- CODEGEN's results are quite different on Android and iOS.
- CODEGEN reads data from APP_PROJECT to `schema.json` which is later used to generate code.
- CODEGEN code is located in `@react-native/codegen`
- `@react-native/codegen` uses template-based code generation (instead of AST-based code generation)


#### `package.json::codegenConfig`

CODEGEN needs to extract information about what should be generated. That configuration needs to be specified in `package.json::codegenConfig`.

```ts
type CodegenConfig = {
  type: "modules" | "components" | "all", 
  name: string | `RTN${string}Spec`,
  jsSrcsDir: string // The relative path to access the js specification that is parsed by Codegen.
  android?: {
    javaPackageName: string
  }
  windows?: {}
  harmony?: {} // Potential Harmony-specific configuration will be placed here for consistency reasons
}
```

### react-native-windows
- CODEGEN code is in `@react-native-windows/codegen` package
- CODEGEN is executed by a CLI command
- Uses template-based approach to code generation
- Currently only supports reading Turbo Modules in Flow.
- Windows-specific configuration is placed in codegenConfig.windows

## Implementation on various platforms
| Type                          | Android                                                                                                  | iOS                                                                                                          | react-native-windows           | Harmony (suggestion)                                                         |
| ----------------------------- | -------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ------------------------------ | ---------------------------------------------------------------------------- |
| CODEGEN manual execution      | `cd <APP_PROJECT_ROOT>/android && gradlew generateCodegenArtifactsFromSchema`                            | `<APP_PROJECT_ROOT>/node_modules/react-native/scripts/generate-codegen-artifacts.js`                         | `react-native codegen-windows` | `react-native codegen-harmony`                                               |
| PACKAGEs generated output dir | `<APP_PROJECT_ROOT>/node_modules/<PACKAGE>/android/build/generated/source/codegen`                       | `<USER_SPECIFIED_OUTPUT>/build/generated/ios`                                                                | ?                              | `<APP_PROJECT_ROOT>/harmony/entry/src/main/ets/codegen`                      |
| Package abstract factory      | `<APP_PROJECT_ROOT>/android/app/build/generated/rncli/src/main/java/com/facebook/react/PackageList.java` | `<APP_PROJECT_ROOT>/ios/Pods/Headers/Private/React-RCTFabric/React/RCTThirdPartyFabricComponentsProvider.h`? | ?                              | `<APP_PROJECT_ROOT>/harmony/entry/src/main/ets/codegen/RNPackagesFactory.ts` |