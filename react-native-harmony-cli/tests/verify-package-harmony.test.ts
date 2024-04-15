import { ReactNativeFixture } from './ReactNativeFixture';

it('list verify-package-harmony in the help description', async () => {
  const result = new ReactNativeFixture('.').help();

  expect(result.includes('verify-package-harmony')).toBeTruthy();
});
