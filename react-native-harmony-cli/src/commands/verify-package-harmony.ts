import { Command } from '@react-native-community/cli-types';
import { DescriptiveError, AbsolutePath } from '../core';
import { Logger } from '../io';
import { CheckResult, verifyNPMPackage } from '../package-verifier';

export const commandVerifyPackageHarmony: Command = {
  name: 'verify-package-harmony',
  description: 'Validates that a package is compliant with guidelines',
  options: [
    {
      name: '--package-path <path>',
      description: 'Path to the npm package',
    },
    {
      name: '--no-harmony-code [boolean]',
      description: 'Disable har verification',
    },
    {
      name: '--skip-checks <string>',
      description: 'Disable certain checks by their names separated by ";"',
    },
  ],
  func: async (argv, config, rawArgs: any) => {
    const logger = new Logger();
    try {
      const args = validateArgs(rawArgs);
      logger.info(
        () => `Validating NPM package: ${args.packagePath.getValue()}`,
        {
          prefix: true,
        }
      );
      const checkResults: CheckResult[] = [];
      for await (const checkResult of verifyNPMPackage({
        npmPackagePath: args.packagePath,
        hasHarmonyCode: args.hasHarmonyCode,
        checkNamesToSkip: args.checkNamesToSkip,
      })) {
        checkResults.push(checkResult);
        logCheckResult(logger, checkResult);
      }
      logger.info(() => '');
      if (
        checkResults.every(
          (checkResult) =>
            checkResult.status === 'pass' || checkResult.status === 'skip'
        )
      ) {
        logger.info((styles) => styles.bold('OK'));
      } else {
        logger.info((styles) =>
          styles.red(
            'Package verification failed. Please fix those checks before publishing your package.'
          )
        );
        process.exit(1);
      }
    } catch (err) {
      if (err instanceof DescriptiveError) {
        logger.descriptiveError(err);
        process.exit(1);
      }
      throw err;
    }
  },
};

type Args = {
  packagePath: AbsolutePath;
  hasHarmonyCode: boolean;
  checkNamesToSkip: string[];
};

function validateArgs(args: any): Args {
  if (!args.packagePath) {
    throw new DescriptiveError({
      whatHappened: 'package-path is undefined',
      whatCanUserDo: { default: ['Please provide --package-path'] },
    });
  }
  return {
    packagePath: new AbsolutePath(args.packagePath),
    hasHarmonyCode: args.harmonyCode,
    checkNamesToSkip: ((args.skipChecks ?? '') as string).split(';'),
  };
}

function logCheckResult(logger: Logger, checkResult: CheckResult) {
  logger.info((styles) => {
    const prefix = (() => {
      switch (checkResult.status) {
        case 'pass':
          return `[${styles.green('PASS')}]`;
        case 'fail':
          return `[${styles.red('FAIL')}]`;
        case 'skip':
          return `[${styles.yellow('SKIP')}]`;
      }
    })();
    return checkResult.message
      ? `${styles.bold(prefix)} ${styles.bold(checkResult.name)}\n${
          checkResult.message
        }`
      : `${styles.bold(prefix)} ${styles.bold(checkResult.name)}`;
  });
}
