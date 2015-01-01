#include "mainwindow.h"

MainWindow::MainWindow(Ui::MainWindowDialog *_d, UiHelper *_ui) :
    QMainWindow(0),
    ui(*_ui)
{
    _d->setupUi(this);
    ui.setup(_d);
    handler.setup(&ui);
}

MainWindow::~MainWindow() {
}
