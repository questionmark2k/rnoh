import fs from 'node:fs';
import { AbsolutePath } from './AbsolutePath';

export function maybeMkDirs(path: AbsolutePath): void {
  if (!fs.existsSync(path.getValue())) {
    fs.mkdirSync(path.getValue(), { recursive: true });
  }
}
