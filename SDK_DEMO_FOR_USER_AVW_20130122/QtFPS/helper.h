#ifndef HELPER_H
#define HELPER_H

#include "ui_mainwindow.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QMainWindow>

class Helper {
public:
    enum ImageType {
        Image256,
        Image320,
        None
    };

private:
    QImage image256;
    QImage image320;

    QGraphicsPixmapItem  *graphicsItem;
    QGraphicsScene *scene;
    ImageType lastType;

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
    explicit Helper();
    virtual ~Helper();

    void setup(Ui::MainWindowDialog *mw);

    void disableOnConnected();
    void enableOnDisconnected();

    void disableOnLive();
    void enableOnLive();

    void drawImage(void *data, ImageType type);

    void saveLastImage(const QString &filename);
};

#endif // HELPER_H
