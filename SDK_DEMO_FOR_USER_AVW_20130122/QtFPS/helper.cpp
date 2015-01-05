#include "helper.h"

#include <QFileDialog>
#include <QMessageBox>

Helper::Helper() :
    image256(256, 256, QImage::Format_Indexed8),
    image320(320, 240, QImage::Format_Indexed8),

    graphicsItem(NULL),
    scene(NULL),

    lastType(None)
{
    for(int i = 0; i < 256; i++) {
        image256.setColor(i, qRgb(i,i,i));
        image320.setColor(i, qRgb(i,i,i));
    }
}

Helper::~Helper() {

    if (graphicsItem) {
        delete graphicsItem;
    }
    if (scene) {
        delete scene;
    }
}

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

void Helper::disableOnConnected() {
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
}

void Helper::enableOnDisconnected() {
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
}


void Helper::disableOnLive() {
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
}

void Helper::enableOnLive() {
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

    scene = canvas->scene();

    if (!scene) {
        qDebug() << "There's no scene!";
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
        qDebug() << "Error, cannot save last image because there's no such image";
    }
}

void Helper::setResult(const QString &line1, const QString &line2) {
    QString r = line1 + "\n" + line2;
    qDebug() << "setResult:" << r;
    result->setText(r);
}

int Helper::getComPort() {
    QString text = serialPortNumber->currentText();
    bool conv = true;
    int p = text.toLower().toInt(&conv);
    if (!conv) {
        qDebug() << "Error while converting COM port text to integer!!!! using default value of 1";
        p = 1;
    }
    return p;
}

int Helper::getComBaudrate() {
    QString text = baudrate->currentText();
    bool conv = true;
    int b = text.toLower().toInt(&conv);
    if (!conv) {
        qDebug() << "Error while converting COM baudrate text to integer!!!! using default value of 9600";
        b = 9600;
    }
    return b;
}

int Helper::getId() {
    int i = id->value();
    if (i < 0 || i > 199) {
        qDebug() << "Id out of range!" << i << "; using nearest value";
    }
    return qMin(199, qMax(0, i));
}

bool Helper::confirm(const QString &message, const QString &title) {
    int result = QMessageBox::warning(0,
        title,
        message,
        QMessageBox::Ok | QMessageBox::Cancel
    );

    return (result == QMessageBox::Ok);
}

QString Helper::getOpenFilename(const QString &exts) {
    return QFileDialog::getOpenFileName(
        0,
        QObject::tr("Open File"),
        QObject::tr("."),
        exts
    );
}

QString Helper::getSaveFilename(const QString &exts, const QString &suggested) {
    return QFileDialog::getSaveFileName(
        0,
        QObject::tr("Save File"),
        suggested,
        exts
    );
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

Params::Params(const QHash<QString,QString> &_map) :
    map(_map)
{
}

Params::Params() {

}

int Params::getInt(const QString &key, int _default) {
    if(map.contains(key)) {
        bool b = true;
        int t = map[key].toInt(&b);
        if (!b) {
            return _default;
        }
        return t;
    }
    return _default;
}
Params& Params::setInt(const QString &key, int value) {
    map[key] = QString::number(value);
    return *this;
}

QString Params::getString(const QString &key, const QString &_default) {
    if(map.contains(key)) {
        return map[key];
    }
    return _default;
}
Params& Params::setString(const QString &key, const QString &value) {
    map[key] = value;
    return *this;
}
