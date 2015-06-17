#include <QApplication>

#include "mainwindow.h"
#include "helper.h"
#include "commbase_qt.h"

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    Ui::MainWindowDialog u;

    QtCommSerial serial_port;

    GuiHelper h;
    Controller c(&serial_port);

    MainWindow w(&u, &h, &c);
    w.show();

    return a.exec();
}
