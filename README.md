# React Native Open Harmony

## How to add `harmony` platform to your React Native project

1. Follow the instructions in the [Prerequisites](#prerequisites) section
1. Copy `tester/.npmrc` to your project
1. Create access token and set `RNOH_NPM_TOKEN` by following instructions above
1. Install react-native-harmony: `npm i react-native-harmony@npm:@rnoh/react-native-harmony`
1. Copy `metro.config.js` to your project
1. Create a new Harmony project in `<YOUR_PROJECT>/harmony`
1. Copy `tester/harmony/entry` module to `<YOUR_PROJECT>/harmony/entry`
1. Run `./node_modules/.bin/react-native unpack-harmony` - it may take a while
1. Add `"postinstall": "react-native unpack-harmony"` to `package.json`'s script section
1. Update `tester/harmony/entry/src/main/ets/pages/Index.ets`
   1. Unregister `SampleView` and `SamplePackage`
   1. Update "app_name" to match the name specified in `<YOUR_PROJECT>/app.json::name`
1. Update `tester/harmony/entry/src/main/cpp/CMakeLists.txt`
   1. Update `RNOH_CPP_DIR` - `set(RNOH_CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../node_modules/react-native-harmony/harmony/cpp")`
   1. Unregister `SamplePackage`
1. Open `harmony` project in DevEcoStudio and run the project

## Architecture

![](./docs/react-native-open-harmony-app--components-diagram.png)

Note: native code lives in the `/tester/harmony/rnoh` directory (native code must be in the project directory).

## Prerequisites

### DevEco Studio setup

1. Download DevEco Studio from [File Storage repo](https://gl.swmansion.com/rnoh/file-storage).
1. Unpack all .partX.rar archives (you can use `The Unarchiver` app).
1. Install DevEco Studio and run it.
1. Close DevEco.
1. In `~/Library/Application\ Support/Huawei/DevEcoStudio4.1/options/country.region.xml`, change `name="PL"` to `name="CN"`.

#### SDK setup

Before starting the development you have to install required SDKs.

##### Installing SDK

1. In DevEco Studio go to `Preferences->SDK`.
1. Choose a location and install SDK.
1. Go to `{DEVECO SDK LOCATION}/HarmonyOS-NEXT-DP1/base` and remove all folders.
1. Download latest [daily SDK build](https://ci.openharmony.cn/workbench/cicd/dailybuild/dailylist) and unpack it.
1. Move all `.zip` files to `{DEVECO SDK LOCATION}/HarmonyOS-NEXT-DP1/base` and unzip them.
1. Remove `.zip` files.
1. Restart DevEco Studio
1. DevEcoStudio: `Build` -> `Clean Project`
1. DevEcoStudio: `File` -> `Sync and Refresh Project`

### Git setup

Before you begin, you have to install the git client on your computer. Refer to [the git documentation](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git) for more details about installation.

In order to download project dependencies from the git repository, you will have to create the access token and save it in environment variables:

#### Creating the access token on GitLab

1. Press the `Edit profile` button on GitLab (press on your avatar on the navbar and then on the button)
1. Select `Access Tokens` tab in the menu on the left side
1. Fill the `Token name` field (e.g. it can be `rnoh`) and select the `api` scope in the `Select scopes` form
1. Press the `Create personal access token` button to create the token
1. Copy the token

#### Saving the access token in environment variables

##### macOS

1. Edit `.zshrc`.
1. Add `export RNOH_NPM_TOKEN={YOUR TOKEN}`.
1. Save it and reload terminals.

##### Windows

1. Press `Win + R` to open the Windows Run prompt
1. Type in `sysdm.cpl` and click `Ok`
1. Go to the `Advanced` tab
1. Press on the `Environment Variables` button
1. Add the new user variable by pressing the `New` button under the User variables section
1. Set variable name to `RNOH_NPM_TOKEN` and paste your access token in the variable value field

More details about GitLab access tokens can be found [in the docs](https://docs.gitlab.com/ee/user/profile/personal_access_tokens.html)

### Cloning the repository

1. On the [project repository](https://gl.swmansion.com/rnoh/react-native-harmony) page press the clone button and copy the url to clone the project
1. In your terminal go to the directory where you want to store project files
1. Run the `git clone --recurse-submodules <url>` command to clone the repository with all submodules (replace the `<url>` with the copied url from the repository page on GitLab)

## Running `tester` app

### Installing Dependencies

1. Go to the `react-native-harmony` directory
1. Create the package by running the `npm install` and `npm pack` command
1. Go to the `react-native-harmony-cli` directory
1. Run the `npm install` and `npm pack` command
1. Go to the `react-native-harmony-sample-package` directory
1. Run the `npm install` and `npm pack` command
1. Go to the `../tester` directory
1. Run the `npm run i` command to install dependencies and setup build-profile

### Creating the Local Emulator (up to 3.1.0 DevEco Studio version only)

Before you start the project, you have to setup the device on which you will be able to run the application. Up to 3.1.0 version of the DevEco Studio IDE, you can create the Local Emulator to test the app on your computer.

1. Open the `Device Manager` (press the dropdown near the run button (green triangle) on the top ribbon)
1. Press on the `Device Manger` option on the list
1. Press the `Install` button to install the emulator SDK
1. Click on the `Finish` button after installation
1. Press the `New emulator` button in the bottom right corner
1. Go through the emulator setup. You can customize the emulator settings
1. You will see the success message popup if the emulator is successfully created

### Running the project on the Local Emulator (up to 3.1.0 DevEco Studio version only)

1. Open the `Device Manager` and start the emulator
1. Press the `Run entry` button in the top ribbon to build the project and run on the emulator
1. Wait for the build to finish

### Running `tester` app using the Metro Bundler

1. Go to the `/tester`
1. Run `npm i`
1. Run `npm run dev`
1. Open `/tester/harmony` in DevEco Studio
1. Start the HarmonyOS emulator
1. Run `hdc rport tcp:8081 tcp:8081`
   If `hdc` is not in your `PATH`, it can be found under `{SDK LOCATION}/HarmonyOS-NEXT-DP1/base/toolchains`
1. Start metro by running `npm run start`
1. Build and run entry module

## Creating NPM Package

1. Go to the `/react-native-harmony`
1. Run `npm run pack:prod`

## Enable C-API

1. Add `RNOH_C_API_ARCH="1"` to environment variables.
1. In `tester/harmony/entry/src/main/ets/pages/Index.ets` change `enableCAPIArchitecture` to `true`
1. Restart DevEco Studio
1. DevEcoStudio: `Build` -> `Clean Project`
1. DevEcoStudio: `File` -> `Sync and Refresh Project`
