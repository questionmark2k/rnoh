import pathUtils from 'node:path';
import fs from 'node:fs';
import { AbsolutePath } from '../core';

export function findFilePathsWithExtensions(
  dirPath: AbsolutePath,
  fileExtensions: readonly string[]
): AbsolutePath[] {
  let results: AbsolutePath[] = [];
  const files = fs.readdirSync(dirPath.getValue(), { withFileTypes: true });
  for (const file of files) {
    const path = dirPath.copyWithNewSegment(file.name);
    if (file.isDirectory()) {
      results = results.concat(
        findFilePathsWithExtensions(path, fileExtensions)
      );
    } else if (
      fileExtensions.includes(pathUtils.extname(file.name).substring(1))
    ) {
      results.push(path);
    }
  }
  return results;
}
