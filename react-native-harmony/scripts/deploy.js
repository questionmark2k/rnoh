// @ts-check
const { execSync } = require('child_process');
const fs = require('node:fs');
const readline = require('readline');

const HAR_FILE_OUTPUT_PATH =
  'tester/harmony/react_native_openharmony/build/default/outputs/default/react_native_openharmony.har';

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});

/**
 * @returns {boolean}
 */
function isRepositoryClean() {
  const status = execSync('git status --porcelain', { encoding: 'utf-8' });
  const branch = execSync('git branch --show-current', { encoding: 'utf-8' });
  const isUpdated = !execSync('git rev-list HEAD...origin/main --count', {
    encoding: 'utf-8',
  });

  return !status && branch === 'main' && isUpdated;
}

function runDeployment() {
  if (!process.cwd().endsWith('react-native-harmony')) {
    console.log(
      'This script should be executed from react-native-harmony directory'
    );
    process.exit(1);
  }

  if (!isRepositoryClean()) {
    console.log(
      'Repository should be clean, on main branch and up to date with upstream.'
    );
    process.exit(1);
  }

  let version = '';

  const currentVersion = JSON.parse(
    fs.readFileSync('./package.json').toString()
  )['version'];
  rl.question(
    `Current version: ${currentVersion}. Enter new version: `,
    (newVersion) => {
      version = newVersion;

      execSync(`npm run update_version  -- --new-version ${version}`, {
        stdio: 'inherit',
      });

      rl.question(
        `Please generate ${HAR_FILE_OUTPUT_PATH} file. Open DevEco Studio, select any file in 'react_native_openharmony' module, and run Build > Make Module 'react_native_openharmony'.\nOnce you finish type 'done': `,
        (answer) => {
          if (answer !== 'done') {
            console.log('Deployment aborted');
            process.exit(1);
          }
          console.log(
            `Copying ${`../${HAR_FILE_OUTPUT_PATH}`} to ./harmony dir`
          );
          if (!fs.existsSync(`../${HAR_FILE_OUTPUT_PATH}`)) {
            console.log(`Couldn't find ${HAR_FILE_OUTPUT_PATH}.`);
            process.exit(1);
          }
          fs.rmSync('./harmony', { recursive: true, force: true });
          fs.mkdirSync('./harmony');
          fs.renameSync(
            `../${HAR_FILE_OUTPUT_PATH}`,
            './harmony/react_native_openharmony.har'
          );

          const changelogForCurrentVersion = execSync(
            `npm run -s gen:changelog`
          ).toString();

          updateChangelog(version, changelogForCurrentVersion);

          execSync(`npm publish --dry-run`, { stdio: 'inherit' });

          rl.question(
            'Are changes good to be published and pushed to the upstream? (yes/no): ',
            (answer) => {
              if (answer.toLowerCase() === 'yes') {
                execSync(`npm publish`, { stdio: 'inherit' });
                execSync('git add -A');
                execSync(
                  `git commit -m "release: react-native-harmony@${version}"`,
                  {
                    stdio: 'inherit',
                  }
                );
                execSync(`git tag v${version}`, { stdio: 'inherit' });
                execSync(`git push`, { stdio: 'inherit' });
                execSync(`git push -u origin v${version}`, {
                  stdio: 'inherit',
                });
                console.log('Changes published successfully.');

                rl.close();
              } else {
                console.log('Deployment aborted.');
                rl.close();
              }
            }
          );
        }
      );
    }
  );
}

/**
 * @param {string} version
 *  @param {string} changelogForCurrentVersion
 */
function updateChangelog(version, changelogForCurrentVersion) {
  let data = fs.readFileSync('../CHANGELOG.md').toString();
  data = data.replace(
    '# Changelog',
    `# Changelog\n\n## v${version}\n ${changelogForCurrentVersion}`
  );
  fs.writeFileSync('../CHANGELOG.md', data);
}

runDeployment();
