#include "helper.h"

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

