#include "console_application.h"

#include <QDebug>
#include <QString>
#include <QStringList>

#include "argsparser.h"
#include "helper.h"
#include "controller.h"

ConsoleApplication::ConsoleApplication(QCoreApplication *coreApp) {
    coreApplication = coreApp;
}

int ConsoleApplication::execute() {
    ArgsParser parser;

    parser.prepare(*coreApplication);
    Config conf = parser.validate();

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
