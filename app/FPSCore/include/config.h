#ifndef CONFIG_H
#define CONFIG_H

typedef signed char int8;          /* 8 bit signed */
typedef unsigned char uint8;       /* 8 bit unsigned */
typedef unsigned char uchar;       /* 8 bit signed */

typedef short int16;               /* 16 bit signed */
typedef unsigned short uint16;     /* 16 bit unsigned */
typedef unsigned short ushort;     /* 16 bit unsigned */

typedef int int32;                 /* 32 bit signed */
typedef unsigned int uint32;       /* 32 bit unsigned */
typedef unsigned int uint;         /* 32 bit unsigned */

typedef long long int64;           /* 64 bit signed */
typedef unsigned long long uint64; /* 64 bit unsigned */

#include <string>

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
//#define QT_SERIAL_COMM
//#define RS232_SERIAL_COMM

class Config {
public:
    std::string port;
    uint baudrate;

    uint id;

    std::string filename;

    uint action;
};

#endif // CONFIG_H
