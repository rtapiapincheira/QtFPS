#ifndef CONSOLE_APPLICATION_H
#define CONSOLE_APPLICATION_H

#include "config.h"
#include "commbase.h"

#include <QCoreApplication>
#include <QtCore>

class ConsoleApplication {
private:
    QCoreApplication *coreApplication;
    CCommSerial *serialPort;
    void (*handler)(void* param);

public:
    ConsoleApplication(QCoreApplication *coreApp, CCommSerial *serialPort, void (*handler)(void* param));

    int execute();
};

#endif // CONSOLE_APPLICATION_H
