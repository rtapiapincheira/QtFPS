#include "config.h"

#ifdef MODE_WINDOW
#   include <QApplication>
#   include "mainwindow.h"
#else
#   include <QCoreApplication>
#endif

#include "helper.h"

int main(int argc, char *argv[])
{
#ifdef MODE_WINDOW
    QApplication a(argc, argv);

    Ui::MainWindowDialog u;
#else
    QCoreApplication a(argc, argv);
#endif

    Helper h;
    Controller c;

    MainWindow w(&u, &h, &c);
    w.show();

    return a.exec();
}
