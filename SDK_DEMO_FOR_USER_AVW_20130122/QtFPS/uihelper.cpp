#include "uihelper.h"

#include <QFile>

UiHelper::UiHelper() {
}

UiHelper::~UiHelper() {

}

void UiHelper::setup(Ui::MainWindowDialog *mw) {
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

void UiHelper::disableOnConnected() {
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

void UiHelper::enableOnDisconnected() {
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

void UiHelper::drawImage(void *data, int x, int y, int w, int h) {

    /*
    QImage img = QImage::fromData((const uchar*)data, w*h);
    if (img.isNull()) {
        qDebug() << "NULL image!";
    } else {
        qDebug() << "Non null image!";
    }
    */
    QFile output("output.txt");
    output.open(QIODevice::ReadWrite);

    QTextStream ss(&output);

    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            ss << (uint)(((uchar *)data)[y*w+x]) << " ";
        }
        ss << "\n";
    }

    output.close();
}
