#include "helper.h"

#ifdef MODE_WINDOW
#  include <QFileDialog>
#  include <QMessageBox>
#endif

Helper::Helper() :
    image256(256, 256, QImage::Format_Indexed8),
    image320(320, 240, QImage::Format_Indexed8),

#ifdef MODE_WINDOW
    graphicsItem(NULL),
    scene(NULL),
#endif

    lastType(None)
{
    for(int i = 0; i < 256; i++) {
        image256.setColor(i, qRgb(i,i,i));
        image320.setColor(i, qRgb(i,i,i));
    }
}

Helper::~Helper() {
#ifdef MODE_WINDOW
    if (graphicsItem) {
        delete graphicsItem;
    }
    if (scene) {
        delete scene;
    }
#endif
}

#ifdef MODE_WINDOW
void Helper::setup(Ui::MainWindowDialog *mw) {
    canvas = mw->canvas;

    saveImageToFile = mw->save_image_to_file;

    result = mw->result;

    serialPortNumber = mw->serial_port_number;
    baudrate = mw->baudrate;

    open = mw->open;
    close = mw->close;

    id = mw->id;

    enroll = mw->enroll;
    getUserCount = mw->get_user_count;
    verify = mw->verify_1_1;
    deleteId = mw->delete_id;
    identify = mw->identify_1_n;
    deleteAll = mw->delete_all;
    verifyTemplate = mw->verify_template;
    getTemplate = mw->get_template;
    identifyTemplate = mw->identify_template;
    setTemplate = mw->set_template;
    isPressedFinger = mw->is_press_finger;
    getDatabase = mw->get_database;

    getImage = mw->get_image;
    setDatabase = mw->set_database;
    getRawImage = mw->get_raw_image;
    cancel = mw->cancel;

    getLiveImage = mw->get_live_image;
}
#endif

void Helper::disableOnConnected() {
#ifdef MODE_WINDOW
    serialPortNumber->setEnabled(false);
    baudrate->setEnabled(false);

    open->setEnabled(false);
    close->setEnabled(true);

    id->setEnabled(true);
    enroll->setEnabled(true);
    getUserCount->setEnabled(true);
    verify->setEnabled(true);
    deleteId->setEnabled(true);
    identify->setEnabled(true);
    deleteAll->setEnabled(true);
    verifyTemplate->setEnabled(true);
    getTemplate->setEnabled(true);
    identifyTemplate->setEnabled(true);
    setTemplate->setEnabled(true);
    isPressedFinger->setEnabled(true);
    getDatabase->setEnabled(true);
    getImage->setEnabled(true);
    setDatabase->setEnabled(true);
    getRawImage->setEnabled(true);

    getLiveImage->setEnabled(true);
#endif
}

void Helper::enableOnDisconnected() {
#ifdef MODE_WINDOW
    serialPortNumber->setEnabled(true);
    baudrate->setEnabled(true);

    open->setEnabled(true);
    close->setEnabled(false);

    id->setEnabled(false);
    enroll->setEnabled(false);
    getUserCount->setEnabled(false);
    verify->setEnabled(false);
    deleteId->setEnabled(false);
    identify->setEnabled(false);
    deleteAll->setEnabled(false);
    verifyTemplate->setEnabled(false);
    getTemplate->setEnabled(false);
    identifyTemplate->setEnabled(false);
    setTemplate->setEnabled(false);
    isPressedFinger->setEnabled(false);
    getDatabase->setEnabled(false);
    getImage->setEnabled(false);
    setDatabase->setEnabled(false);
    getRawImage->setEnabled(false);

    getLiveImage->setEnabled(false);
#endif
}

void Helper::disableOnLive() {
#ifdef MODE_WINDOW
    close->setEnabled(false);

    id->setEnabled(false);
    enroll->setEnabled(false);
    getUserCount->setEnabled(false);
    verify->setEnabled(false);
    deleteId->setEnabled(false);
    identify->setEnabled(false);
    deleteAll->setEnabled(false);
    verifyTemplate->setEnabled(false);
    getTemplate->setEnabled(false);
    identifyTemplate->setEnabled(false);
    setTemplate->setEnabled(false);
    isPressedFinger->setEnabled(false);
    getDatabase->setEnabled(false);
    getImage->setEnabled(false);
    setDatabase->setEnabled(false);
    getRawImage->setEnabled(false);

    cancel->setEnabled(true);

    getLiveImage->setEnabled(false);
#endif
}

void Helper::enableOnLive() {
#ifdef MODE_WINDOW
    close->setEnabled(true);

    id->setEnabled(true);
    enroll->setEnabled(true);
    getUserCount->setEnabled(true);
    verify->setEnabled(true);
    deleteId->setEnabled(true);
    identify->setEnabled(true);
    deleteAll->setEnabled(true);
    verifyTemplate->setEnabled(true);
    getTemplate->setEnabled(true);
    identifyTemplate->setEnabled(true);
    setTemplate->setEnabled(true);
    isPressedFinger->setEnabled(true);
    getDatabase->setEnabled(true);
    getImage->setEnabled(true);
    setDatabase->setEnabled(true);
    getRawImage->setEnabled(true);

    cancel->setEnabled(false);

    getLiveImage->setEnabled(true);
#endif
}

void Helper::drawImage(void *data, ImageType type) {
    QImage *image = &image256;

    if(type == Image256) {
        image = &image256;
        lastType = Image256;
    } else if(type == Image320) {
        image = &image320;
        lastType = Image320;
    }

    int w = image->width();
    int h = image->height();

    // Set the colors
    for(int y = 0; y<h; y++){
        for(int x = 0; x<w; x++){
            int i = y*w+x;
            int c = (int) ((uchar*)data)[i];
            image->setPixel(x,y, c);
        }
    }

#ifdef MODE_WINDOW
    scene = canvas->scene();

    if (!scene) {
#ifdef OUTPUT_DEBUG
        qDebug() << "There's no scene!";
#endif
        scene = new QGraphicsScene;
        canvas->setScene(scene);
    }

    if (graphicsItem) {
        canvas->scene()->removeItem(graphicsItem);
        delete graphicsItem;
    }

    canvas->setFixedSize(w, h);
    canvas->scene()->setSceneRect(0, 0, w, h);

    graphicsItem = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
    canvas->scene()->addItem(graphicsItem);
#endif
}

void Helper::saveLastImage(const QString &_filename) {
    QString filename = _filename;
    if(!filename.toLower().endsWith(".png")) {
        filename += ".png";
    }

    if (lastType == Image256) {
        image256.save(filename);
    } else if (lastType == Image320) {
        image320.save(filename);
    } else {
#ifdef OUTPUT_DEBUG
        qDebug() << "Error, cannot save last image because there's no such image";
#endif
    }
}

void Helper::setResult(const QString &line1, const QString &line2) {
    QString r = line1 + "\n" + line2;
#ifdef OUTPUT_DEBUG
    qDebug() << "setResult:" << r;
#else
    QTextStream ss(stdout);
    ss << ">> " << line1.trimmed() << endl;
    if (!line2.trimmed().isEmpty()) {
        ss << ">> " << line2.trimmed() << endl;
    }
#endif
#ifdef MODE_WINDOW
    result->setText(r);
#else
    Q_UNUSED(line1);
    Q_UNUSED(line2);
#endif
}

QString Helper::getComPort() {
#ifdef MODE_WINDOW
    return serialPortNumber->currentText();
#else
    return consoleConfig.port;
#endif
}

int Helper::getComBaudrate() {
#ifdef MODE_WINDOW
    QString text = baudrate->currentText();
    bool conv = true;
    int b = text.toLower().toInt(&conv);
    if (!conv) {
#ifdef OUTPUT_DEBUG
        qDebug() << "Error while converting COM baudrate text to integer!!!! using default value of 9600";
#endif
        b = 9600;
    }
    return b;
#else
    return consoleConfig.baudrate;
#endif
}

int Helper::getId() {
#ifdef MODE_WINDOW
    int i = id->value();
    if (i < 0 || i > 199) {
#ifdef OUTPUT_DEBUG
        qDebug() << "Id out of range!" << i << "; using nearest value";
#endif
    }
    return qMin(199, qMax(0, i));
#else
    return consoleConfig.id;
#endif
}

bool Helper::confirm(const QString &message, const QString &title) {
#ifdef MODE_WINDOW
    int result = QMessageBox::warning(0,
        title,
        message,
        QMessageBox::Ok | QMessageBox::Cancel
    );

    return (result == QMessageBox::Ok);
#else
    Q_UNUSED(message);
    Q_UNUSED(title);
#ifdef OUTPUT_DEBUG
    qDebug() << "Because this is a console app, returning fixed value (" << message << ", " << title << ")";
#endif
    return true;
#endif
}

QString Helper::getOpenFilename(const QString &exts) {
#ifdef MODE_WINDOW
    return QFileDialog::getOpenFileName(
        0,
        QObject::tr("Open File"),
        QObject::tr("."),
        exts
    );
#else
    Q_UNUSED(exts);
#ifdef OUTPUT_DEBUG
    qDebug() << "Because this is a console app, returning fixed value (" << exts << ")";
#endif
    return consoleConfig.filename;
#endif
}

QString Helper::getSaveFilename(const QString &exts, const QString &suggested) {
#ifdef MODE_WINDOW
    return QFileDialog::getSaveFileName(
        0,
        QObject::tr("Save File"),
        suggested,
        exts
    );
#else
    Q_UNUSED(exts);
    Q_UNUSED(suggested);
#ifdef OUTPUT_DEBUG
    qDebug() << "Because this is a console app, returning fixed value (" << exts << "," << suggested << ")";
#endif
    return consoleConfig.filename;
#endif
}

qint64 Helper::writeAll(const QString &filename, uchar *buffer, uint size) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        qint64 written = 0;
        while(written < size) {
            qint64 w = file.write((char*)(buffer+written), size-written);
            if (w < 0) {
                break;
            }
            written += w;
        }
        file.close();
        return written;
    }
    return -1;
}

qint64 Helper::readAll(const QString &filename, uchar *buffer, uint size) {
    QFile file(filename);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        qint64 read = 0;
        while(read < size) {
            qint64 r = file.read((char*)(buffer+read), size-read);
            if(r < 0) {
                break;
            }
            read += r;
        }
        file.close();
        return read;
    }
    return -1;
}

QString Helper::formatSerialNumber(uchar *p) {
    QString sn = "";
    for(int i = 0; i < 16; i++) {
        sn += (p[i] < 16 ? "0" : "") + QString::number(p[i], 16);
        if(i == 7) {
            sn += "-";
        }
    }
    return sn.toUpper();
}
