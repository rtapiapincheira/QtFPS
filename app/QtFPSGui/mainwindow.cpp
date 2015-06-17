#include "mainwindow.h"

void uiPolling(void *p) {
    qApp->processEvents();
}

MainWindow::MainWindow(Ui::MainWindowDialog *_d, GuiHelper *_ui, Controller *c) :
    QMainWindow(0),
    ui(*_ui),
    controller(*c)
{
    _d->setupUi(this);
    ui.setup(_d);

    controller.setup(&ui, uiPolling);
}

MainWindow::~MainWindow() {
}
