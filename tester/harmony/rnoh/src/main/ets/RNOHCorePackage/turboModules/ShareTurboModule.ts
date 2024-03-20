import type {TurboModuleContext} from '../../RNOH/TurboModule';
import {TurboModule} from '../../RNOH/TurboModule';
import {RNOHLogger} from '../../RNOH/RNOHLogger';
import wantConstant from '@ohos.app.ability.wantConstant';

const ACTION_SHARED = 'sharedAction';

export class ShareTurboModule extends TurboModule {
  public static readonly NAME = 'ShareModule';
  private logger: RNOHLogger;

  constructor(protected ctx: TurboModuleContext) {
    super(ctx);
    this.logger = this.ctx.logger.clone('ShareTurboModule');
  }

  async share(
    content: {title?: string; message?: string; url?: string},
    dialogTitle?: string,
  ): Promise<{action: string}> {
    try {
      const want = {
        action: 'ohos.want.action.sendData',
        type: 'text/plain',
        parameters: {
          [wantConstant.Params.SHARE_ABSTRACT_KEY]: content.message,
          [wantConstant.Params.SHARE_URL_KEY]: content.url,
          [wantConstant.Params.CONTENT_TITLE_KEY]: content.title,
        },
      };
      await this.ctx.uiAbilityContext.startAbility(want); // no information from API to check if sharing was successful
      this.logger.info('startAbility success');
    } catch (error) {
      this.logger.error('startAbility returned error: ' + error); // sometimes startAbility returns InternalError but shares the content correctly
    }
    return Promise.resolve({action: ACTION_SHARED});
  }
}
