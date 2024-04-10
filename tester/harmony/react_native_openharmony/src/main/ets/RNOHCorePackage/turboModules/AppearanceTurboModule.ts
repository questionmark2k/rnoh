import type { TurboModuleContext } from "../../RNOH/TurboModule";
import { TurboModule } from "../../RNOH/TurboModule";
import ConfigurationConstant from '@ohos.app.ability.ConfigurationConstant';
import type EnvironmentCallback from '@ohos.app.ability.EnvironmentCallback';

type JSColorScheme = 'light' | 'dark' | null;

export class AppearanceTurboModule extends TurboModule {
  public static readonly NAME = 'Appearance';
  private lastColorScheme: JSColorScheme = null;
  /**
   * This is a hack. Currently applicationContext.setColorMode(colorMode) doesn't seem to work for COLOR_MODE_NOT_SET.
   * We store the initialColorMode so when the JS side wants to set colorScheme to null, we can set the ColorMode to the
   * ColorMode which was used on app startup (which was set in system settings).  (colorScheme set to null means that
   * the colorMode which is set in the system settings should be used).
   * Refer to RN docs: https://reactnative.dev/docs/appearance
   */
  private initialColorMode: ConfigurationConstant.ColorMode = null;

  constructor(protected ctx: TurboModuleContext) {
    super(ctx);
    this.setup();
  }

  private colorModeToJSColorScheme(colorMode: ConfigurationConstant.ColorMode): JSColorScheme {
    if (colorMode === ConfigurationConstant.ColorMode.COLOR_MODE_LIGHT) {
      return 'light'
    } else if (colorMode === ConfigurationConstant.ColorMode.COLOR_MODE_DARK) {
      return 'dark';
    } else {
      return null;
    }
  }

  getColorScheme(): JSColorScheme {
    return this.lastColorScheme;
  }

  setup(): void {
    this.initialColorMode = this.ctx.uiAbilityContext.config.colorMode
    this.lastColorScheme = this.colorModeToJSColorScheme(this.initialColorMode);

    let envCallback: EnvironmentCallback = {
      onConfigurationUpdated: (config) => {
        const colorMode = config.colorMode;
        this.lastColorScheme = this.colorModeToJSColorScheme(colorMode);

        this.ctx.rnInstance.emitDeviceEvent("appearanceChanged", {
          colorScheme: this.lastColorScheme
        });
      },

      onMemoryLevel: () => {
        //we need this empty callback, otherwise it won't compile
      }
    };

    const applicationContext = this.ctx.uiAbilityContext.getApplicationContext();
    applicationContext.on('environment', envCallback);
  }

  setColorScheme(colorScheme: string): void {
    const applicationContext = this.ctx.uiAbilityContext.getApplicationContext();
    let colorMode: ConfigurationConstant.ColorMode;
    if (colorScheme === 'light') {
      colorMode = ConfigurationConstant.ColorMode.COLOR_MODE_LIGHT;
    } else if (colorScheme === 'dark') {
      colorMode = ConfigurationConstant.ColorMode.COLOR_MODE_DARK;
    } else {
      //for null we want to use the colorMode set in the system settings
      colorMode = this.initialColorMode;
    }


    applicationContext.setColorMode(colorMode);
  };
}
