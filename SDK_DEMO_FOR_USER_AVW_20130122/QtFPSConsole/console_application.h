#ifndef CONSOLE_APPLICATION_H
#define CONSOLE_APPLICATION_H

#include "config.h"

#include <QCoreApplication>
#include <QtCore>

class ConsoleApplication {
private:
    QCoreApplication *coreApplication;

public:
    ConsoleApplication(QCoreApplication *coreApp);

    int execute();
};

#endif // CONSOLE_APPLICATION_H
