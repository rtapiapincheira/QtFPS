#include <QCoreApplication>

#include "console_application.h"

#include "commbase_rs232.h"

void uiPolling(void *p) {
    qApp->processEvents();
}

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);

    Rs232CommSerial serial_port;

    ConsoleApplication consoleApp(&a, &serial_port, &uiPolling);

    return consoleApp.execute();
}
