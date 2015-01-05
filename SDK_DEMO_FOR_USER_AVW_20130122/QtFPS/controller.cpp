#include "controller.h"

#include "oem/oem.h"

Oem oem;

LedLocker::LedLocker() {
    oem.cmosLed(true);
}

LedLocker::~LedLocker() {
    oem.cmosLed(false);
}

void handlerUiPolling(void *parameter) {
    Q_UNUSED(parameter);
#ifdef MODE_WINDOW
    qApp->processEvents();
#endif
}

Controller::Controller(QObject *parent) :
    QObject(parent)
{
}

void Controller::setResult(const QString &line1, const QString &line2) {
    ui->setResult(line1, line2);
    handlerUiPolling();
}

void Controller::setup(Helper *_ui) {
    ui = _ui;

    connect(ui->open, SIGNAL(clicked()), this, SLOT(__open()));
    connect(ui->close, SIGNAL(clicked()), this, SLOT(__close()));

    // Exclusive amongst them!
    connect(ui->enroll, SIGNAL(clicked()), this, SLOT(__enroll()));
    connect(ui->getUserCount, SIGNAL(clicked()), this, SLOT(__getUserCount()));
    connect(ui->verify, SIGNAL(clicked()), this, SLOT(__verify()));
    connect(ui->deleteId, SIGNAL(clicked()), this, SLOT(__deleteId()));
    connect(ui->identify, SIGNAL(clicked()), this, SLOT(__identify()));
    connect(ui->deleteAll, SIGNAL(clicked()), this, SLOT(__deleteAll()));
    connect(ui->verifyTemplate, SIGNAL(clicked()), this, SLOT(__verifyTemplate()));
    connect(ui->getTemplate, SIGNAL(clicked()), this, SLOT(__getTemplate()));
    connect(ui->identifyTemplate, SIGNAL(clicked()), this, SLOT(__identifyTemplate()));
    connect(ui->setTemplate, SIGNAL(clicked()), this, SLOT(__setTemplate()));
    connect(ui->isPressedFinger, SIGNAL(clicked()), this, SLOT(__isPressedFinger()));
    connect(ui->getDatabase, SIGNAL(clicked()), this, SLOT(__getDatabase()));
    connect(ui->setDatabase, SIGNAL(clicked()), this, SLOT(__setDatabase()));

    connect(ui->getImage, SIGNAL(clicked()), this, SLOT(__getImage()));
    connect(ui->getRawImage, SIGNAL(clicked()), this, SLOT(__getRawImage()));
    connect(ui->getLiveImage, SIGNAL(clicked()), this, SLOT(__getLiveImage()));

    // Optional, in addition to three above
    connect(ui->saveImageToFile, SIGNAL(clicked()), this, SLOT(__saveImageToFile()));

    connect(ui->cancel, SIGNAL(clicked()), this, SLOT(__cancel()));
}

void Controller::__saveImageToFile() {
    qDebug() << "handleSaveImageToFile";

    QString suggested = QDateTime::currentDateTime().toString("yyyyMMddThhmmss.png");

    QString filename = ui->getSaveFilename(EXTENSION_IMAGE, suggested);
    if(filename.isEmpty()) {
        setResult("Save image canceled!");
        return;
    }

    ui->saveLastImage(filename);
}

void Controller::__open() {
    qDebug() << "handleOpenClicked";

    int port = ui->getComPort();

    qDebug() << "Opening port:" << port;

    int baud = ui->getComBaudrate();
    if(oem.openPort(port, baud) < 0){
        qDebug() << "Device is not connected to serial port.";
        return;
    }

    if(oem.openOem() < 0) {
        qDebug() << "Cannot connect to the device !";
        return;
    } if(oem.gwLastAck == NACK_INFO) {
        qDebug() << "Cannot connect to the device !";
        return;
    }

    setResult(
        QString("FirmwareVersion: %1, IsoAreaMaxSize: %2 KB")
            .arg(QString::number(oem.gDevInfo.FirmwareVersion, 16))
            .arg(oem.gDevInfo.IsoAreaMaxSize / 1024),
        QString("DeviceSN: %1")
            .arg(ui->formatSerialNumber(oem.gDevInfo.DeviceSerialNumber))
    );

    ui->disableOnConnected();
}

void Controller::__close() {
    qDebug() << "handleCloseClicked";

    oem.closePort();

    setResult("");

    ui->enableOnDisconnected();
}

void Controller::__enroll() {
    qDebug() << "handleEnrollClicked";

    LedLocker ll;
    Q_UNUSED(&ll);

    int nID = ui->getId();

    if(oem.enrollStart(nID) < 0) {
        setResult("Communication error!");
        return;
    }

    if(oem.gwLastAck == NACK_INFO) {
        qDebug() << "handleEnrollClicked: error";
        setResult(oem.utilError(oem.gwLastAckParam, nID));
        return;
    }

    for(int i=1; i<=3; i++) {
        setResult(QString("Input finger %1!").arg(i));

        QString err;
        if(oem.utilCapturing(true, err) < 0) {
            setResult(err);
            return;
        }

        qint64 ds = QDateTime::currentMSecsSinceEpoch();

        setResult("Processing fingerprint...");

        if(oem.enrollNth(nID, i) < 0) {
            setResult("Communication error!");
            return;
        }
        if(oem.gwLastAck == NACK_INFO) {
            qDebug() << "this point!";
            setResult(oem.utilError(oem.gwLastAckParam, 0));
            return;
        }

        qint64 dt = QDateTime::currentMSecsSinceEpoch() - ds;

        setResult("Take off finger, please ...");

        if(i<3) {
            while(true) {
                if(oem.isPressFinger() < 0){
                    setResult("Communication error!");
                    return;
                }
                if(oem.gwLastAck == NACK_INFO) {
                    qDebug() << "this other point!";
                    setResult(oem.utilError(oem.gwLastAckParam, 0));
                    return;
                }
                if(oem.gwLastAckParam != 0) {
                    break;
                }
            }
        }
        setResult(QString("Process time: %1 ms").arg(dt));
    }

    setResult(QString("Enroll OK (ID = %1)!").arg(nID));
}

void Controller::__getUserCount() {
    qDebug() << "handleGetUserCountClicked";

    if(oem.enrollCount() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, 0));
        return;
    }

    setResult(QString("Enroll count = %1 !").arg(oem.gwLastAckParam));
}

void Controller::__verify() {
    qDebug() << "handleVerifyClicked";

    int nID = ui->getId();

    if(oem.checkEnrolled(nID) < 0) {
        setResult("Communication error!");
        return;
    }

    if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, nID));
        return;
    }

    LedLocker ll;
    Q_UNUSED(&ll);

    setResult("Input finger !");

    qint64 ds = QDateTime::currentMSecsSinceEpoch();

    QString err;
    if(oem.utilCapturing(false, err) < 0) {
        qDebug() << "my_oem_capturing, err:" << err;
        return;
    }

    setResult("Processing fingerprint...");

    if(oem.verify(nID) < 0) {
        setResult("Communication error!");
        return;
    }

    if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, nID));
        return;
    }

    setResult(QString("ID = %1 (MATCH!) : %2 ms").arg(nID).arg(QDateTime::currentMSecsSinceEpoch() - ds));
}

void Controller::__deleteId() {
    qDebug() << "handleDeleteIdClicked";

    int nID = ui->getId();

    QString message = QString("Are you sure to delete ID %1?").arg(nID);
    if (ui->confirm(message)) {
        if(oem.deleteId(nID) < 0) {
            setResult("Communication error");
            return;
        }
        if (oem.gwLastAck == NACK_INFO) {
            setResult(oem.utilError(oem.gwLastAckParam, nID));
            return;
        }
        setResult("Delete OK!");
    } else {
        setResult("Canceled!");
    }
}

void Controller::__identify() {
    qDebug() << "handleIdentifyClicked";

    if(oem.enrollCount() < 0) {
        setResult("Communication error!");
        return;
    } if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, 0));
        return;
    }

    if (oem.gwLastAckParam == 0) {
        setResult("No user !");
        return;
    }

    LedLocker ll;
    Q_UNUSED(&ll);

    setResult("Input finger !");

    QString err;
    if(oem.utilCapturing(false, err) < 0) {
        setResult(err);
        return;
    }

    qint64 ds = QDateTime::currentMSecsSinceEpoch();

    setResult("Processing fingerprint...");

    if(oem.identify() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, 0));
        return;
    }

    setResult(QString("ID = %1      : %2 ms").arg(oem.gwLastAckParam).arg(QDateTime::currentMSecsSinceEpoch() - ds));
}

void Controller::__deleteAll() {
    qDebug() << "handleDeleteAllClicked";

    if (ui->confirm("Are you sure to delete all?")) {
        if(oem.deleteAll() < 0) {
            setResult("Communication error");
            return;
        }
        if (oem.gwLastAck == NACK_INFO) {
            setResult(oem.utilError(oem.gwLastAckParam, 0));
            return;
        }

        setResult("Delete all OK!");
    } else {
        setResult("Canceled!");
    }
}

void Controller::__verifyTemplate() {
    qDebug() << "handleVerifyTemplateClicked";

    int nID = ui->getId();

    if(oem.checkEnrolled(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, nID));
        return;
    }

    QString filename = ui->getOpenFilename(EXTENSION_TEMPLATE);
    if(filename.isEmpty()) {
        setResult("Open file canceled!");
        return;
    }

    qint64 ds = QDateTime::currentMSecsSinceEpoch();

    qint64 currentRead = ui->readAll(filename, oem.gbyTemplate, FP_TEMPLATE_SIZE);
    if (currentRead < 0) {
        setResult("Cannot open file for read!");
        return;
    }

    if(currentRead != FP_TEMPLATE_SIZE) {
        setResult("Invalid size for read!");
        return;
    }

    if(oem.verifyTemplate(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAck, nID));
        return;
    }

    setResult(QString("ID = %1      : %2 ms").arg(nID).arg(QDateTime::currentMSecsSinceEpoch()-ds));
}

void Controller::__getTemplate() {
    qDebug() << "handleGetTemplateClicked";

    int nID = ui->getId();

    setResult(QString("Getting of %1th template...").arg(nID));

    if(oem.getTemplate(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck==NACK_INFO ) {
        setResult(oem.utilError(oem.gwLastAckParam, nID));
        return;
    }

    //////////////////////////////////////////////////////////////////////////

    QString filename = ui->getSaveFilename(EXTENSION_TEMPLATE);
    if(filename.isEmpty()) {
        setResult("Open file anceled!");
        return;
    }

    qint64 written = ui->writeAll(filename, oem.gbyTemplate, FP_TEMPLATE_SIZE);
    if (written < 0) {
        setResult("Cannot create file for write!");
        return;
    }
    if (written != FP_TEMPLATE_SIZE) {
        setResult("Error while writing file!");
        return;
    }

    //////////////////////////////////////////////////////////////////////////

    setResult(QString("Get Template OK (ID = %1)!").arg(nID));
}

void Controller::__identifyTemplate() {
    qDebug() << "handleIdentifyTemplateClicked";

    if(oem.enrollCount() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, 0));
        return;
    }

    if(oem.gwLastAckParam == 0) {
        setResult("No user !");
        return;
    }

    QString filename = ui->getOpenFilename(EXTENSION_TEMPLATE);
    if(filename.isEmpty()) {
        setResult("Open file canceled!");
        return;
    }

    qint64 ds = QDateTime::currentMSecsSinceEpoch();

    qint64 currentRead = ui->readAll(filename, oem.gbyTemplate, FP_TEMPLATE_SIZE);
    if(currentRead < 0) {
        setResult("Cannot open file for read!");
        return;
    }

    if(currentRead != FP_TEMPLATE_SIZE) {
        setResult("Error while reading the file!");
        return;
    }

    //////////////////////////////////////////////////////////////////////////

    if(oem.identifyTemplate() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, 0));
        return;
    }

    setResult(QString("ID = %1      : %2 ms").arg(oem.gwLastAckParam).arg(QDateTime::currentMSecsSinceEpoch() - ds));
}

void Controller::__setTemplate() {
    qDebug() << "handleSetTemplateClicked";

    int nID = ui->getId();

    QString filename = ui->getOpenFilename(EXTENSION_TEMPLATE);
    if(filename.isEmpty()) {
        setResult("Setting template canceled!");
        return;
    }

    qint64 currentRead = ui->readAll(filename, oem.gbyTemplate, FP_TEMPLATE_SIZE);
    if(currentRead < 0) {
        setResult("Cannot open file for read!");
        return;
    }
    if(currentRead != FP_TEMPLATE_SIZE ) {
        setResult("Invalid size for read!");
        return;
    }

    //////////////////////////////////////////////////////////////////////////

    setResult(QString("Adding of %1th template...").arg(nID));

    if(oem.addTemplate(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck==NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, nID));
        return;
    }

    setResult(QString("Set Template OK (ID = %1)!").arg(nID));
}

void Controller::__isPressedFinger() {
    qDebug() << "handleIsPressedFingerClicked";

    LedLocker ll;
    Q_UNUSED(&ll);

    if(oem.isPressFinger() < 0){
        setResult("Communication error");
        return;
    }
    if(oem.gwLastAck == NACK_INFO){
        setResult(oem.utilError(oem.gwLastAckParam, 0));
        return;
    }
    if(oem.gwLastAckParam != 0) {
        setResult("Finger is not pressed!");
        return;
    }

    setResult("Finger is pressed!");
}

void Controller::__getDatabase() {
    qDebug() << "handleGetDatabaseClicked";

    int nIndexBufPos = FP_TEMPLATE_SIZE * (FP_MAX_USERS+1);

    if(oem.enrollCount() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, 0));
        return;
    }

    if (oem.gwLastAckParam == 0) {
        setResult("No user !");
        return;
    }

    if(oem.getDatabaseStart() < 0){
        setResult("Communication error!");
        return;
    }

    int nEnrollCount = 0;
    for (int i=0; i<FP_MAX_USERS; i++) {
        setResult(QString("Getting of %1th template...").arg(i));

        if(oem.getTemplate(i) < 0) {
             setResult("Communication error!");
             oem.getDatabaseEnd();
             return;
        }
        if(oem.gwLastAck==NACK_INFO && oem.gwLastAckParam!=NACK_IS_NOT_USED ) {
            setResult(oem.utilError(oem.gwLastAckParam, i));
            oem.getDatabaseEnd();
            return;
        }

        if(oem.gwLastAck==NACK_INFO && oem.gwLastAckParam==NACK_IS_NOT_USED ) {
            memset(&oem.gbyTemplateDB[i*FP_TEMPLATE_SIZE], 0xFF, FP_TEMPLATE_SIZE);
            oem.gbyTemplateDB[nIndexBufPos+i] = 0;
        } else {
            memcpy(&oem.gbyTemplateDB[i*FP_TEMPLATE_SIZE], oem.gbyTemplate, FP_TEMPLATE_SIZE);
            oem.gbyTemplateDB[nIndexBufPos+i] = 1;
            nEnrollCount++;
        }
    }
    oem.getDatabaseEnd();

    //////////////////////////////////////////////////////////////////////////
    QString filename = ui->getSaveFilename(EXTENSION_DATABASE);
    if(filename.isEmpty()) {
        setResult("File save canceled!");
        return;
    }

    qint64 currentWritten = ui->writeAll(filename, oem.gbyTemplateDB, FP_TEMPLATE_DB_SIZE);
    if(currentWritten < 0) {
        setResult("Cannot create file for write!");
        return;
    }

    if (currentWritten != FP_TEMPLATE_DB_SIZE) {
        setResult("Error while saving the file!");
        return;
    }

    //////////////////////////////////////////////////////////////////////////

    setResult(QString("Get Database OK (Enroll count = %1)!").arg(nEnrollCount));
}

void Controller::__getImage() {
    qDebug() << "handleGetImageClicked";

    LedLocker ll;
    Q_UNUSED(&ll);

    setResult("Input finger !");

    QString err;
    if(oem.utilCapturing(true, err) < 0) {
        qDebug() << "my_oem_capturing: error";
        setResult(err);
        return;
    }
    if(oem.utilLoadingImage(err) < 0) {
        qDebug() << "my_oem_loading_image: error";
        setResult(err);
        return;
    }

    ui->drawImage(oem.gbyImg8bit, Helper::Image256);
    setResult("Get Image OK !");

    ui->saveImageToFile->setEnabled(true);
}

void Controller::__setDatabase() {
    qDebug() << "handleSetDatabaseClicked";

    // Ask for a database filename!
    QString filename = ui->getOpenFilename(EXTENSION_DATABASE);
    if(filename.isEmpty()) {
        return;
    }

    qint64 currentRead = ui->readAll(filename, oem.gbyTemplateDB, FP_TEMPLATE_DB_SIZE);
    if(currentRead < 0) {
        setResult("Cannot open file for read!");
        return;
    }

    // Read the whole file!
    if(currentRead != FP_TEMPLATE_DB_SIZE) {
        setResult("Invalid size for read!");
        return;
    }

    // Search the index of the first valid user!
    int nIndexBufPos = FP_TEMPLATE_SIZE * (FP_MAX_USERS+1);
    int j;
    for(j=0; j<FP_MAX_USERS; j++) {
        if(oem.gbyTemplateDB[nIndexBufPos+j]) {
            break;
        }
    }

    if(j == FP_MAX_USERS) {
        setResult("No user !");
        return;
    }
    //////////////////////////////////////////////////////////////////////////

    int nAddCount = 0;
    for (int i=0; i<FP_MAX_USERS; i++) {

        if(oem.gbyTemplateDB[nIndexBufPos+i] == 0) {
            continue;
        }

        setResult(QString("Adding of %1th template...").arg(i));

        memcpy(oem.gbyTemplate, &oem.gbyTemplateDB[i*FP_TEMPLATE_SIZE], FP_TEMPLATE_SIZE);

        if(oem.addTemplate(i) < 0) {
            setResult("Communication error!");
            return;
        }
        if(oem.gwLastAck == NACK_INFO) {
            setResult(oem.utilError(oem.gwLastAckParam, i));
            return;
        }
        nAddCount++;
    }

    setResult(QString("Set Database OK (Enroll count = %1)!").arg(nAddCount));
}

void Controller::__getRawImage() {
    qDebug() << "handleGetRawImageClicked";

    LedLocker ll;
    Q_UNUSED(&ll);

    setResult("Input finger !");

    QString err;
    if(oem.utilCapturing(true, err) < 0) {
        qDebug() << "my_oem_capturing: error";
        setResult(err);
        return;
    }
    if(oem.utilLoadingImageRaw(err) < 0) {
        qDebug() << "my_oem_loading_image_raw: error";
        setResult(err);
        return;
    }

    ui->drawImage(oem.gbyImgRaw, Helper::Image320);
    setResult("Get Image OK !");

    ui->saveImageToFile->setEnabled(true);
}

void Controller::__cancel() {
    qDebug() << "handleCancelClicked";

    bContinue = false;
}

void Controller::__getLiveImage() {
    qDebug() << "handleGetLiveImageClicked";

    ui->disableOnLive();

    LedLocker ll;
    Q_UNUSED(&ll);

    bContinue = true;

    setResult("Downloading Live Image ...");

    QString err;

    while(bContinue) {
        if (oem.utilLoadingImageLive(err) < 0) {
            qDebug() << "my_oem_loading_image_live: error";
            setResult(err);
            break;
        }
        handlerUiPolling();

        ui->drawImage(oem.gbyImgRaw, Helper::Image320);
        handlerUiPolling();
    }

    setResult("Download of Live Image finished");

    ui->enableOnLive();
}
