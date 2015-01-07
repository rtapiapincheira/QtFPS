#include "argsparser.h"

#define QT4_HELP_TEXT "Usage: ./app COM1 115200 enroll print.png 1"

QStringList ArgsParser::actions = QStringList()
        << "enroll"
        << "getUserCount"
        << "verify"
        << "deleteId"
        << "identify"
        << "deleteAll"
        << "verifyTemplate"
        << "getTemplate"
        << "identifyTemplate"
        << "setTemplate"
        << "isPressedFinger"
        << "getDatabase"
        << "getImage"
        << "setDatabase"
        << "getRawImage"
        << "getLiveImage"
;

void ArgsParser::maybeExit(bool cond, int ret) {
    if (cond) {
#ifdef ENABLED_QT5
        parser.showHelp(ret);
#else
        printf(QT4_HELP_TEXT);
        printf("\r\n");

        QCoreApplication::exit(ret);
#endif
    }
}

void ArgsParser::prepare(QCoreApplication &a) {
    QCoreApplication::setApplicationName("QtFPSConsole");
    QCoreApplication::setApplicationVersion("1.0");

#ifdef ENABLED_QT5
    //parser = QCommandLineParser();

    parser.setApplicationDescription("Command line application for fingerprint GT-511C3");

    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("port", "Port for the serial connection(COM1, /dev/tty2, etc.)");
    parser.addPositionalArgument("baudrate", "One of 9600, 19200, 38400, 57600 or 115200");
    parser.addPositionalArgument("action", "One of " + actions.join(", "));
    parser.addPositionalArgument("filename", "Name of the file (something.png)");
    parser.addPositionalArgument("id", "Id of a user (from 0 to 199 inclusive)");

    parser.process(a);
    args = parser.positionalArguments();
#else
    args = a.arguments();
    if (args.size() > 0) {
        args.takeFirst();
    }
#endif
}

Config ArgsParser::validate() {
    Config config;

    bool state;

    // Port parsing
    state = true;
    maybeExit(args.size() < 1);
    QString p = args[0];

    // Baudrate parsing
    state = true;
    maybeExit(args.size() < 2);
    int b = args[1].toInt(&state);
    maybeExit(!state || (b!=9600 && b!=19200 && b!=38400 && b!=57600 && b!=115200));

    // Action parsing
    maybeExit(args.size() < 3);
    QString a = args[2];
    maybeExit(actions.indexOf(a) == -1);

    // TODO: make some actions depend on specific parameters (id, filename), not all of them!

    // Filename parsing
    maybeExit(args.size() < 4);
    QString f = args[3];
    maybeExit(f.isEmpty());

    // Id parsing
    state = true;
    maybeExit(args.size() < 5);
    int i = args[4].toInt(&state);
    maybeExit(!state || i < 0 || i > 199);

    config.port = p;
    config.baudrate = b;
    config.action = actions.indexOf(a);
    config.id = i;
    config.filename = f;

    return config;
}
