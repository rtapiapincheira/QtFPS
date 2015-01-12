#include <QApplication>

#include "mainwindow.h"
#include "helper.h"

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    Ui::MainWindowDialog u;

    Helper h;
    Controller c;

    MainWindow w(&u, &h, &c);
    w.show();

    return a.exec();
}
