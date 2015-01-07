#ifndef CONFIG_H
#define CONFIG_H

// this is defined at compiler level, to avoid polluting of widgets other apps
/**
  * If defined, enables GUI widgets in the application (please also enable in the .pro file). If
  * disabled, it allows the Helper class to be used along with console applications. Beware that
  * the .pro files can be configured from this file and will require additional configuration to
  * enable/disable some components.
  *
  * TODO: investigate an automagical way of doing this.
  */
//#define MODE_WINDOW


/**
  * If defined, enables verbose debug output for troubleshooting. Please disable it for console
  * applications as it will get mixed with other console outputs. For WINDOW_MODE it's okay, but it
  * may present performance issues because it will spit out huge amounts of data to the console.
  */
//#define OUTPUT_DEBUG

#endif // CONFIG_H
