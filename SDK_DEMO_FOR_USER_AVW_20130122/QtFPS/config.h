#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore>

// this is defined at compiler level, to avoid polluting other apps
/**
  * If defined, enables GUI widgets in the application (please also enable it in the .pro file). If
  * disabled, it allows the Helper class to be used along with console applications. Beware that
  * the .pro files can't be configured from this file and will require additional configuration to
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

// this is defined at compiler level, to avoid polluting other apps
/**
  * If defined, will include some Qt5 specific headers like QCommandLineParser and QtSerialPort. If
  * undefined, provide a proper Qt4 code replacement instead.
  */
//#define ENABLED_QT5


class Config {
public:
    QString port;
    uint baudrate;

    uint id;

    QString filename;

    int action;

    QString toString() {
        return QString("filename:%1, id:%2, port:%3, baudrate:%4, action:%5")
            .arg(filename)
            .arg(id)
            .arg(port)
            .arg(baudrate)
            .arg(action)
        ;
    }
};

#endif // CONFIG_H
