import fs from 'node:fs';
import { AbsolutePath } from './AbsolutePath';
import { PackageJSON } from './PackageJSON';

export class ProjectDependency {
  constructor(
    private packageRootPath: AbsolutePath,
    private projectRootPath: AbsolutePath
  ) {}

  getRootPath() {
    return this.packageRootPath;
  }

  maybeCreateCodegenConfig() {
    return this.readPackageJSON().maybeCreateCodegenConfig();
  }

  readPackageJSON() {
    return PackageJSON.fromProjectRootPath(
      this.packageRootPath,
      this.projectRootPath
    );
  }
}

export class ProjectDependenciesManager {
  constructor(private projectRootPath: AbsolutePath) {}

  async forEachAsync(
    cb: (dependency: ProjectDependency) => Promise<void> | void
  ) {
    await this.forEachDependencyInDirectory(
      cb,
      this.projectRootPath.copyWithNewSegment('node_modules')
    );
  }

  private async forEachDependencyInDirectory(
    cb: (dependency: ProjectDependency) => Promise<void> | void,
    directoryPath: AbsolutePath
  ) {
    if (!fs.existsSync(directoryPath.getValue())) {
      return;
    }
    for (let dirOrFileName of fs.readdirSync(directoryPath.getValue())) {
      const potentialDependencyRootPath =
        directoryPath.copyWithNewSegment(dirOrFileName);
      if (fs.lstatSync(potentialDependencyRootPath.getValue()).isDirectory()) {
        if (dirOrFileName.startsWith('.')) {
          continue;
        } else if (dirOrFileName.startsWith('@')) {
          await this.forEachDependencyInDirectory(
            cb,
            directoryPath.copyWithNewSegment(dirOrFileName)
          );
        } else {
          if (
            !fs.existsSync(
              potentialDependencyRootPath
                .copyWithNewSegment('package.json')
                .getValue()
            )
          ) {
            continue;
          }
          await cb(
            new ProjectDependency(
              potentialDependencyRootPath,
              this.projectRootPath
            )
          );
        }
      }
    }
  }
}
