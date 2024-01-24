import fs from 'node:fs';
import { AbsolutePath } from './AbsolutePath';
import { DescriptiveError } from './DescriptiveError';

export function maybeMakeDirectories(path: AbsolutePath): void {
  if (!fs.existsSync(path.getValue())) {
    fs.mkdirSync(path.getValue(), { recursive: true });
  }
}

export function maybeRemoveFilesInDirectory(path: AbsolutePath): void {
  if (!fs.existsSync(path.getValue())) {
    return;
  }
  if (!fs.lstatSync(path.getValue()).isDirectory()) {
    throw new DescriptiveError({
      whatHappened:
        "Tried to remove all files from a path that doesn't point to a directory",
      whatCanUserDo: { default: [`Verify path: ${path.getValue()}`] },
    });
  }
  const fileOrDirNames = fs.readdirSync(path.getValue());
  fileOrDirNames.forEach((fileOrDirName) => {
    const maybeFilePath = path.copyWithNewSegment(fileOrDirName);
    if (fs.statSync(maybeFilePath.getValue()).isDirectory()) {
      return;
    } else {
      fs.unlinkSync(maybeFilePath.getValue());
    }
  });
}
