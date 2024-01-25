import fs from 'node:fs';
import { AbsolutePath } from '../../core';
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

  private readPackageJSON() {
    return PackageJSON.fromProjectRootPath(
      this.packageRootPath,
      this.projectRootPath
    );
  }
}

export class ProjectDependenciesManager {
  constructor(private projectRootPath: AbsolutePath) {}

  forEach(cb: (dependency: ProjectDependency) => void) {
    this.forEachDependencyInDirectory(
      cb,
      this.projectRootPath.copyWithNewSegment('node_modules')
    );
  }

  private forEachDependencyInDirectory(
    cb: (dependency: ProjectDependency) => void,
    directoryPath: AbsolutePath
  ) {
    if (!fs.existsSync(directoryPath.getValue())) {
      return;
    }
    fs.readdirSync(directoryPath.getValue()).map((dirOrFileName) => {
      const potentialDependencyRootPath =
        directoryPath.copyWithNewSegment(dirOrFileName);
      if (fs.lstatSync(potentialDependencyRootPath.getValue()).isDirectory()) {
        if (dirOrFileName.startsWith('.')) {
          return;
        } else if (dirOrFileName.startsWith('@')) {
          this.forEachDependencyInDirectory(
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
            return;
          }
          cb(
            new ProjectDependency(
              potentialDependencyRootPath,
              this.projectRootPath
            )
          );
        }
      }
    });
  }
}
