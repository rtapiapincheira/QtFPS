#include <QApplication>

#include "mainwindow.h"
#include "helper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Ui::MainWindowDialog u;
    Helper h;

    MainWindow w(&u, &h);
    w.show();

    return a.exec();
}
