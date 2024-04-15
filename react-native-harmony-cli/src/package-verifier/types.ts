export type CheckStatus = 'pass' | 'fail' | 'skip';

export type Check = {
  checkIf: string;
  run: () => Promise<CheckStatus | { status: CheckStatus; message: string }>;
};

export type CheckResult = {
  name: string;
  status: CheckStatus;
  message: string;
};
