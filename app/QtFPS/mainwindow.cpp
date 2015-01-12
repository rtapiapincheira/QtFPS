#include "mainwindow.h"

MainWindow::MainWindow(Ui::MainWindowDialog *_d, Helper *_ui, Controller *c) :
    QMainWindow(0),
    ui(*_ui),
    controller(*c)
{
    _d->setupUi(this);
    ui.setup(_d);

    controller.setup(&ui);
}

MainWindow::~MainWindow() {
}
