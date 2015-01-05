#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helper.h"
#include "handler.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Ui::MainWindowDialog *ui, Helper *h);
    ~MainWindow();

private:
    Helper &ui;
    Handler handler;
};

#endif // MAINWINDOW_H
