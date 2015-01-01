#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "uihelper.h"
#include "uihandler.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Ui::MainWindowDialog *ui, UiHelper *h);
    ~MainWindow();

private:
    UiHelper &ui;
    UiHandler handler;
};

#endif // MAINWINDOW_H
