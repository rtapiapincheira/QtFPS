#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helper.h"
#include "controller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Ui::MainWindowDialog *ui, Helper *h, Controller *c);
    ~MainWindow();

private:
    Helper &ui;
    Controller &controller;
};

#endif // MAINWINDOW_H
