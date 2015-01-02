#ifndef UIHELPER_H
#define UIHELPER_H

#include "ui_mainwindow.h"

#include <QDebug>
#include <QMainWindow>

class UiHelper {
private:


public:
    QGraphicsView *canvas;

    QPushButton *saveImageToFile;

    QLabel *result;

    QComboBox *serialPortNumber;
    QComboBox *baudrate;

    QPushButton *open;
    QPushButton *close;

    QSpinBox *id;

    QPushButton *enroll;
    QPushButton *getUserCount;
    QPushButton *verify;
    QPushButton *deleteId;
    QPushButton *identify;
    QPushButton *deleteAll;
    QPushButton *verifyTemplate;
    QPushButton *getTemplate;
    QPushButton *identifyTemplate;
    QPushButton *setTemplate;
    QPushButton *isPressedFinger;
    QPushButton *getDatabase;
    QPushButton *getImage;
    QPushButton *setDatabase;
    QPushButton *getRawImage;
    QPushButton *cancel;

    QPushButton *getLiveImage;

public:
    explicit UiHelper();
    virtual ~UiHelper();

    void setup(Ui::MainWindowDialog *mw);

    void disableOnConnected();
    void enableOnDisconnected();

    void drawImage(void *data, int x, int y, int w, int h);
};

#endif // UIHELPER_H
