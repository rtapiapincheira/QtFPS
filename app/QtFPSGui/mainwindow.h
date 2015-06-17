#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "guihelper.h"
#include "controller.h"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Ui::MainWindowDialog *ui, GuiHelper *h, Controller *c);
    ~MainWindow();

private:
    GuiHelper &ui;
    Controller &controller;
};

#endif // MAINWINDOW_H
