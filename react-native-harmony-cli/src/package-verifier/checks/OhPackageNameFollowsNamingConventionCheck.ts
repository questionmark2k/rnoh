import { Check } from '../types';

export function createOhPackageNameFollowsNamingConventionCheck(
  npmPackageName: string,
  ohPackageName: string
): Check {
  const npmPackageNameInfo = getPackageNameInfo(npmPackageName);
  return {
    checkIf: 'oh_package_name_is_derived_from_npm_package_name',
    run: async () => {
      if (npmPackageNameInfo.organizationName) {
        const expectedOHPackageName = `@rnoh/${npmPackageNameInfo.organizationName}--${npmPackageNameInfo.unscopedName}`;
        return expectedOHPackageName === ohPackageName
          ? 'pass'
          : {
              status: 'fail',
              message: `expected '${expectedOHPackageName}', got: ${ohPackageName}`,
            };
      } else {
        const expectedOHPackageName = `@rnoh/${npmPackageNameInfo.unscopedName}`;
        return expectedOHPackageName === ohPackageName
          ? 'pass'
          : {
              status: 'fail',
              message: `expected '${expectedOHPackageName}', got: ${ohPackageName}`,
            };
      }
    },
  };
}

type PackageNameInfo = {
  organizationName: string | null;
  unscopedName: string;
};

function getPackageNameInfo(packageName: string): PackageNameInfo {
  if (packageName.startsWith('@') && packageName.includes('/')) {
    const [part1, part2] = packageName.split('/');
    return {
      organizationName: part1.replace('@', ''),
      unscopedName: part2,
    };
  } else {
    return { organizationName: null, unscopedName: packageName };
  }
}
