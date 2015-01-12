#ifndef HELPER_H
#define HELPER_H

#include "config.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QImage>

#ifdef MODE_WINDOW
#  include "ui_mainwindow.h"
#  include <QGraphicsPixmapItem>
#  include <QMainWindow>
#endif

#define EXTENSION_DATABASE "Database Files (*.db *.dat)"
#define EXTENSION_TEMPLATE "Template Files (*.dat)"
#define EXTENSION_IMAGE    "Image Files (*.png)"

class Helper {
public:
    enum ImageType {
        Image256,
        Image320,
        None
    };

    Config consoleConfig;

private:

    QImage image256;
    QImage image320;
    ImageType lastType;

#ifdef MODE_WINDOW
    QGraphicsPixmapItem  *graphicsItem;
    QGraphicsScene *scene;
#endif

public:

#ifdef MODE_WINDOW
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
    QPushButton *getLiveImage;
    QPushButton *cancel;
#endif

public:
    explicit Helper();
    virtual ~Helper();

#ifdef MODE_WINDOW
    void setup(Ui::MainWindowDialog *mw);
#endif

    void disableOnConnected();
    void enableOnDisconnected();

    void disableOnLive();
    void enableOnLive();

    void drawImage(void *data, ImageType type);

    void saveLastImage(const QString &filename);

    void setResult(const QString &line1, const QString &line2="");

    QString getComPort();
    int getComBaudrate();
    int getId();

    bool confirm(const QString &message, const QString &title="Confirmation");
    QString getOpenFilename(const QString &exts);
    QString getSaveFilename(const QString &exts, const QString &suggested=".");

    qint64 writeAll(const QString &filename, uchar *buffer, uint size);
    qint64 readAll(const QString &filename, uchar *buffer, uint size);

    QString formatSerialNumber(uchar *serialNum);
};

#endif // HELPER_H
