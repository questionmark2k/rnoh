import chalk from 'chalk';
import { DescriptiveError } from '../core';

export class Logger {
  info(
    prepareMsg: (styles: typeof chalk) => string,
    options?: { prefix?: boolean }
  ) {
    const prefix = options?.prefix ? `[${chalk.bold(chalk.cyan('INFO'))}]` : '';
    if (prefix) {
      console.info(prefix, prepareMsg(chalk));
    } else {
      console.info(prepareMsg(chalk));
    }
  }

  debug(prepareMsg: (styles: typeof chalk) => string) {
    console.debug(prepareMsg(chalk));
  }

  descriptiveError(error: DescriptiveError) {
    const lines: string[] = [];
    if (error.isUnexpected() && error.stack) {
      lines.push('');
      lines.push(chalk.bold('Stack'));
      error.stack.split('\n').forEach((line) => lines.push(chalk.gray(line)));
    }
    const details = error.getDetails();
    if (details) {
      lines.push('');
      lines.push(chalk.bold('Details'));
      details.split('\n').forEach((line) => lines.push(line));
    }
    lines.push('');
    let prefix = '';
    if (error.isUnexpected()) {
      prefix = '[' + chalk.red(chalk.bold('UNEXPECTED_ERROR')) + ']';
    } else {
      prefix = '[' + chalk.red(chalk.bold('ERROR')) + ']';
    }
    lines.push(`${prefix} ${error.getMessage()}`);
    const suggestionsByRole = error.getSuggestionsByRole();
    if (suggestionsByRole) {
      if ((suggestionsByRole.default?.length ?? 0) > 0) {
        lines.push('');
        lines.push(chalk.bold(`Suggestions`));
        suggestionsByRole.default?.forEach((suggestion) => {
          lines.push(suggestion);
        });
      }
      if ((suggestionsByRole.rnAppDeveloper?.length ?? 0) > 0) {
        lines.push('');
        lines.push(chalk.bold(`Suggestions for RN App developers`));
        suggestionsByRole.rnAppDeveloper?.forEach((suggestion) => {
          lines.push(suggestion);
        });
      }
      if (suggestionsByRole.rnPackageDeveloper?.length ?? 0) {
        lines.push('');
        lines.push(chalk.bold(`Suggestions for RN package developers`));
        suggestionsByRole.rnPackageDeveloper?.forEach((suggestion) => {
          lines.push(suggestion);
        });
      }
    }
    console.error(lines.join('\n'));
  }
}
