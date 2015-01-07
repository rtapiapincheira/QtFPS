#include <QCoreApplication>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QString>
#include <QStringList>

#include "helper.h"
#include "controller.h"

QStringList actions = QStringList()
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

void prepareParser(QCommandLineParser &parser) {
    QCoreApplication::setApplicationName("QtFPSConsole");
    QCoreApplication::setApplicationVersion("1.0");

    parser.setApplicationDescription("Command line application for fingerprint GT-511C3");

    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("port", "Port for the serial connection(COM1, /dev/tty2, etc.)");
    parser.addPositionalArgument("baudrate", "One of 9600, 19200, 38400, 57600 or 115200");
    parser.addPositionalArgument("action", "One of " + actions.join(", "));
    parser.addPositionalArgument("filename", "Name of the file (something.png)");
    parser.addPositionalArgument("id", "Id of a user (from 0 to 199 inclusive)");
}

Config validateParser(QCommandLineParser &parser) {
    Config config;

    QStringList args = parser.positionalArguments();

    bool state;

    // Port parsing
    state = true;
    if (args.size() < 1) {
        parser.showHelp(1);
    }
    QString p = args[0];

    // Baudrate parsing
    state = true;
    if (args.size() < 2) {
        parser.showHelp(1);
    }
    int b = args[1].toInt(&state);
    if (!state || (b!=9600 && b!=19200 && b!=38400 && b!=57600 && b!=115200)) {
        parser.showHelp(1);
    }

    // Action parsing
    if (args.size() < 3) {
        parser.showHelp(1);
    }
    QString a = args[2];
    if (actions.indexOf(a) == -1) {
        parser.showHelp(1);
    }

    // TODO: make some actions depend on specific parameters (id, filename), not all of them!

    // Filename parsing
    if (args.size() < 4) {
        parser.showHelp(1);
    }
    QString f = args[3];
    if (f.isEmpty()) {
        parser.showHelp(1);
    }

    // Id parsing
    state = true;
    if (args.size() < 5) {
        parser.showHelp(1);
    }
    int i = args[4].toInt(&state);
    if (!state || i < 0 || i > 199) {
        parser.showHelp(1);
    }

    config.port = p;
    config.baudrate = b;
    config.action = actions.indexOf(a);
    config.id = i;
    config.filename = f;

    return config;
}

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    prepareParser(parser);
    parser.process(a);
    Config conf = validateParser(parser);

    Helper h;
    Controller c;
    c.setup(&h);

    /* Note: we need to push the config into Helper h, because from there, those settings will be
     * taken when Controller executes any of its slots (the switch below; controller takes the
     * helper as an initialization parameter).
     */
    h.consoleConfig = conf;

    c.__open();

    switch(h.consoleConfig.action) {
    case 0: c.__enroll(); break;
    case 1: c.__getUserCount(); break;
    case 2: c.__verify(); break;
    case 3: c.__deleteId(); break;
    case 4: c.__identify(); break;
    case 5: c.__deleteAll(); break;
    case 6: c.__verifyTemplate(); break;
    case 7: c.__getTemplate(); break;
    case 8: c.__identifyTemplate(); break;
    case 9: c.__setTemplate(); break;
    case 10:c.__isPressedFinger(); break;
    case 11:c.__getDatabase(); break;
    case 12:c.__getImage(); c.__saveImageToFile(); break;
    case 13:c.__setDatabase(); break;
    case 14:c.__getRawImage(); c.__saveImageToFile(); break;
    case 15:c.__getLiveImage(); break;
    }

    c.__close();

    return 0;
}
