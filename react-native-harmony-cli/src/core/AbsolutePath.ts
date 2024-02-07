import pathUtils from 'node:path';
import { ValueObject } from './ValueObject';

export class AbsolutePath implements ValueObject {
  static fromSegments(...segments: string[]) {
    return new AbsolutePath(pathUtils.join(...segments));
  }

  private value: string = '';
  constructor(path: string) {
    this.value = pathUtils.resolve(path);
  }

  getValue(): string {
    return this.value;
  }

  copyWithNewSegment(...relativePath: string[]): AbsolutePath {
    return new AbsolutePath(pathUtils.join(this.getValue(), ...relativePath));
  }

  getExtension(): string | null {
    const maybeExtWithDot = pathUtils.extname(this.value);
    if (maybeExtWithDot) {
      return maybeExtWithDot.substring(1);
    }
    return null;
  }

  relativeTo(absolutePath: AbsolutePath): RelativePath {
    return new RelativePath(
      pathUtils.relative(absolutePath.getValue(), this.value)
    );
  }
}

class RelativePath implements ValueObject {
  constructor(private value: string) {}

  getValue() {
    return this.value;
  }
}
