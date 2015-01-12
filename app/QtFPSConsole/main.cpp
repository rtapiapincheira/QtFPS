#include <QCoreApplication>

#include "console_application.h"

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);

    ConsoleApplication consoleApp(&a);

    return consoleApp.execute();
}
