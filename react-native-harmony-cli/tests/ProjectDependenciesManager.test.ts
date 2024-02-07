import tmp from 'tmp';
import { ProjectDependenciesManager } from '../src/core/ProjectDependenciesManager';
import { createFileStructure } from './fsUtils';
import { AbsolutePath } from '../src/core';

let tmpDir: string = '';

beforeEach(async () => {
  const dir = tmp.dirSync();
  tmpDir = dir.name;
});

it('should not recognize a dependency if package.json is not defined', async () => {
  createFileStructure(tmpDir, {
    node_modules: {
      '@types': {
        node: {
          'ts.4.8': {},
        },
      },
      'some-package': {
        'package.json': '...',
      },
    },
  });
  const projectDependenciesManager = new ProjectDependenciesManager(
    new AbsolutePath(tmpDir)
  );

  const dependencyRootPaths: AbsolutePath[] = [];
  await projectDependenciesManager.forEachAsync((dependency) => {
    dependencyRootPaths.push(dependency.getRootPath());
  });

  expect(dependencyRootPaths.length).toBe(1);
});
