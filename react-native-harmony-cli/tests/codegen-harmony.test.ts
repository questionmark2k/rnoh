import tmp from 'tmp';
import { ReactNativeFixture } from './ReactNativeFixture';
import fs from 'fs';
import { buildDirTree, createFileStructure } from './fsUtils';
import { AbsolutePath } from '../src/core';
import { RawCodegenConfig } from '../src/codegen';

let tmpDir: string = '';

beforeEach(async () => {
  const dir = tmp.dirSync();
  tmpDir = dir.name;
});

afterEach(() => {
  if (expect.getState().assertionCalls != expect.getState().numPassingAsserts) {
    console.log(buildDirTree(tmpDir));
  }
});

function createPackageJSONContent(config: {
  codegenConfig?: RawCodegenConfig;
}) {
  return JSON.stringify(
    {
      name: 'fake-package',
      version: '0.0.1',
      private: false,
      scripts: {},
      dependencies: {},
      harmony: {
        codegenConfig: config.codegenConfig,
      },
    },
    null,
    2
  );
}

function createTurboModuleSpec(name: string) {
  return `
import type { TurboModule } from 'react-native/Libraries/TurboModule/RCTExport';
import { TurboModuleRegistry } from 'react-native';

interface Spec extends TurboModule {
  runProcedure(): void;
  getBool(arg: boolean): boolean;
  getString(arg: string): string;
  getObject(arg: { x: { y: number } }): Object;
  getArrayOfNumbersAsync(): Promise<number[]>;
}

export default TurboModuleRegistry.get<Spec>("${name}")!;
`;
}

// --------------------------------------------------------------------------------------------------

it("should not generate code for packages that don't specify 'codegenConfig' in their 'package.json'", async () => {
  createFileStructure(tmpDir, {
    harmony: {
      entry: {
        src: {
          main: {
            ets: {
              generated: {},
            },
            cpp: {},
          },
        },
      },
    },
    src: {
      specs: {
        'NativeSampleTurboModuleSpec.ts': createTurboModuleSpec(
          'AppSampleTurboModule'
        ),
      },
    },
    node_modules: {
      'react-native-harmony-sample-package': {
        src: {
          specs: {
            'NativeSampleTurboModuleSpec.ts': createTurboModuleSpec(
              'ThirdPartySampleTurboModule'
            ),
          },
        },
        'package.json': createPackageJSONContent({
          codegenConfig: {
            specPaths: ['./src/specs'],
          },
        }),
      },
    },
    'package.json': createPackageJSONContent({}),
  });

  new ReactNativeFixture(tmpDir).codegenHarmony({
    etsOutputPath: './harmony/entry/src/main/ets/generated',
    cppOutputPath: './harmony/entry/src/main/cpp/generated',
    projectRootPath: '.',
  });

  const cppGeneratedDirPath = AbsolutePath.fromSegments(
    tmpDir,
    'harmony',
    'entry',
    'src',
    'main',
    'cpp',
    'generated'
  );

  expect(
    fs.existsSync(
      cppGeneratedDirPath
        .copyWithNewSegment('ThirdPartySampleTurboModule.h')
        .getValue()
    )
  ).toBeTruthy();
});

it('should allow specifying paths to specs in other packages', async () => {
  createFileStructure(tmpDir, {
    harmony: {
      entry: {
        src: {
          main: {
            ets: {
              generated: {},
            },
            cpp: {},
          },
        },
      },
    },
    src: {
      specs: {
        'NativeSampleTurboModuleSpec.ts': createTurboModuleSpec(
          'AppSampleTurboModule'
        ),
      },
    },
    node_modules: {
      'react-native-sample-package': {
        src: {
          specs: {
            'NativeSampleTurboModuleSpec.ts': createTurboModuleSpec(
              'ThirdPartySampleTurboModule'
            ),
          },
        },
        'package.json': createPackageJSONContent({}),
      },
      'react-native-harmony-sample-package': {
        'package.json': createPackageJSONContent({
          codegenConfig: {
            specPaths: ['~/node_modules/react-native-sample-package/src/specs'],
          },
        }),
      },
    },
    'package.json': createPackageJSONContent({}),
  });

  new ReactNativeFixture(tmpDir).codegenHarmony({
    etsOutputPath: './harmony/entry/src/main/ets/codegen',
    cppOutputPath: './harmony/entry/src/main/cpp/codegen',
    projectRootPath: '.',
  });

  const cppGeneratedDirPath = AbsolutePath.fromSegments(
    tmpDir,
    'harmony',
    'entry',
    'src',
    'main',
    'cpp',
    'codegen'
  );

  expect(
    fs.existsSync(
      cppGeneratedDirPath
        .copyWithNewSegment('ThirdPartySampleTurboModule.h')
        .getValue()
    )
  ).toBeTruthy();
});

describe('module', () => {
  it('should generate glue layer for a simple turbo module in app directory', async () => {
    createFileStructure(tmpDir, {
      harmony: {
        entry: {
          src: {
            main: {
              ets: {
                generated: {},
              },
              cpp: {},
            },
          },
        },
      },
      src: {
        specs: {
          'NativeSampleTurboModuleSpec.ts':
            createTurboModuleSpec('SampleTurboModule'),
        },
      },
      'package.json': createPackageJSONContent({
        codegenConfig: {
          specPaths: ['./src/specs'],
        },
      }),
    });

    new ReactNativeFixture(tmpDir).codegenHarmony({
      etsOutputPath: './harmony/entry/src/main/ets/generated',
      cppOutputPath: './harmony/entry/src/main/cpp/generated',
      projectRootPath: '.',
    });

    const cppGeneratedDirPath = AbsolutePath.fromSegments(
      tmpDir,
      'harmony',
      'entry',
      'src',
      'main',
      'cpp',
      'generated'
    );

    expect(
      fs.existsSync(
        cppGeneratedDirPath.copyWithNewSegment('SampleTurboModule.h').getValue()
      )
    ).toBeTruthy();
    expect(
      fs.existsSync(
        cppGeneratedDirPath
          .copyWithNewSegment('SampleTurboModule.cpp')
          .getValue()
      )
    ).toBeTruthy();
    expect(
      fs.existsSync(
        cppGeneratedDirPath
          .copyWithNewSegment('RNOHGeneratedPackage.h')
          .getValue()
      )
    ).toBeTruthy();
  });
});
