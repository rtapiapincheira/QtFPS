#include <QApplication>

#include "mainwindow.h"
#include "uihelper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Ui::MainWindowDialog u;
    UiHelper h;

    MainWindow w(&u, &h);
    w.show();

    return a.exec();
}
