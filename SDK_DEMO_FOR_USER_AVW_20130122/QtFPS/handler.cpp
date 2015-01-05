#include "handler.h"

#include "oem/oem.h"

Oem oem;

LedLocker::LedLocker() {
    oem.cmosLed(true);
}

LedLocker::~LedLocker() {
    oem.cmosLed(false);
}

void handlerUiPolling(void *parameter) {
    qApp->processEvents();
    //((Handler*)parameter)->handlerUiPolling();
}

Handler::Handler(QObject *parent) :
    QObject(parent)
{
}

void Handler::setResult(const QString &line1, const QString &line2) {
    QString result = line1 + "\n" + line2;
    qDebug() << "setResult:" << result;
    ui->result->setText(result);
}

void Handler::setup(Helper *_ui) {
    ui = _ui;

    connect(ui->saveImageToFile, SIGNAL(clicked()), this, SLOT(handleSaveImageToFileClicked()));
    connect(ui->open, SIGNAL(clicked()), this, SLOT(handleOpenClicked()));
    connect(ui->close, SIGNAL(clicked()), this, SLOT(handleCloseClicked()));
    connect(ui->enroll, SIGNAL(clicked()), this, SLOT(handleEnrollClicked()));
    connect(ui->getUserCount, SIGNAL(clicked()), this, SLOT(handleGetUserCountClicked()));
    connect(ui->verify, SIGNAL(clicked()), this, SLOT(handleVerifyClicked()));
    connect(ui->deleteId, SIGNAL(clicked()), this, SLOT(handleDeleteIdClicked()));
    connect(ui->identify, SIGNAL(clicked()), this, SLOT(handleIdentifyClicked()));
    connect(ui->deleteAll, SIGNAL(clicked()), this, SLOT(handleDeleteAllClicked()));
    connect(ui->verifyTemplate, SIGNAL(clicked()), this, SLOT(handleVerifyTemplateClicked()));
    connect(ui->getTemplate, SIGNAL(clicked()), this, SLOT(handleGetTemplateClicked()));
    connect(ui->identifyTemplate, SIGNAL(clicked()), this, SLOT(handleIdentifyTemplateClicked()));
    connect(ui->setTemplate, SIGNAL(clicked()), this, SLOT(handleSetTemplateClicked()));
    connect(ui->isPressedFinger, SIGNAL(clicked()), this, SLOT(handleIsPressedFingerClicked()));
    connect(ui->getDatabase, SIGNAL(clicked()), this, SLOT(handleGetDatabaseClicked()));
    connect(ui->getImage, SIGNAL(clicked()), this, SLOT(handleGetImageClicked()));
    connect(ui->setDatabase, SIGNAL(clicked()), this, SLOT(handleSetDatabaseClicked()));
    connect(ui->getRawImage, SIGNAL(clicked()), this, SLOT(handleGetRawImageClicked()));
    connect(ui->cancel, SIGNAL(clicked()), this, SLOT(handleCancelClicked()));
    connect(ui->getLiveImage, SIGNAL(clicked()), this, SLOT(handleGetLiveImageClicked()));
}

void Handler::handleSaveImageToFileClicked() {
    qDebug() << "handleSaveImageToFile";

    QString suggested = QDateTime::currentDateTime().toString("yyyyMMddThhmmss.png");

    QString m_strFilePath = QFileDialog::getSaveFileName(0,
         tr("Save Image File"), suggested, tr("Image File (*.png)"));

    if(m_strFilePath.isEmpty()) {
        return;
    }

    ui->saveLastImage(m_strFilePath);
}

void Handler::handleOpenClicked() {
    qDebug() << "handleOpenClicked";

    int port = ui->serialPortNumber->currentText().replace("COM","").toInt();

    qDebug() << "Opening port:" << port;

    int baud = ui->baudrate->currentText().toInt();
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

    uchar* p = oem.gDevInfo.DeviceSerialNumber;

    QString sn = "";
    for(int i = 0; i < 16; i++) {
        sn += (p[i] < 16 ? "0" : "") + QString::number(p[i], 16);
        if(i == 7) {
            sn += "-";
        }
    }

    setResult(
        QString("FirmwareVersion: %1, IsoAreaMaxSize: %2 KB\r\nDeviceSN: %3")
            .arg(QString::number(oem.gDevInfo.FirmwareVersion, 16))
            .arg(oem.gDevInfo.IsoAreaMaxSize / 1024)
            .arg(sn.toUpper())
    );

    ui->disableOnConnected();
}

void Handler::handleCloseClicked() {
    qDebug() << "handleCloseClicked";

    oem.closePort();

    setResult("");

    ui->enableOnDisconnected();
}

void Handler::handleEnrollClicked() {
    qDebug() << "handleEnrollClicked";

    LedLocker ll;
    Q_UNUSED(&ll);

    int nID = ui->id->value();

    if(oem.enrollStart(nID) < 0) {
        setResult("Communication error!");
        return;
    }

    handlerUiPolling();

    if(oem.gwLastAck == NACK_INFO) {
        qDebug() << "handleEnrollClicked: error";
        setResult(oem.utilError(oem.gwLastAckParam, nID));
        return;
    }

    for(int i=1; i<=3; i++) {
        setResult(QString("Input finger %1!").arg(i));
        handlerUiPolling();

        QString err;
        if(oem.utilCapturing(true, err) < 0) {
            setResult(err);
            return;
        }

        qint64 ds = QDateTime::currentMSecsSinceEpoch();

        setResult("Processing fingerprint...");
        handlerUiPolling();

        if(oem.enrollNth(nID, i) < 0) {
            setResult("Communication error!");
            return;
        }
        if(oem.gwLastAck == NACK_INFO) {
            qDebug() << "this point!";
            setResult(oem.utilError(oem.gwLastAckParam, 0));
            return;
        }

        qint64 dt = (QDateTime::currentMSecsSinceEpoch() - ds);

        setResult("Take off finger, please ...");
        handlerUiPolling();

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
        handlerUiPolling();
    }

    setResult(QString("Enroll OK (ID = %1)!").arg(nID));
    handlerUiPolling();
}

void Handler::handleGetUserCountClicked() {
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

void Handler::handleVerifyClicked() {
    qDebug() << "handleVerifyClicked";

    int nID = ui->id->value();

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

    setResult("");
    handlerUiPolling();

    setResult("Input finger !");
    handlerUiPolling();

    qint64 ds = QDateTime::currentMSecsSinceEpoch();

    QString err;
    if(oem.utilCapturing(false, err) < 0) {
        qDebug() << "my_oem_capturing, err:" << err;
        return;
    }

    setResult("Processing fingerprint...");
    handlerUiPolling();

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

void Handler::handleDeleteIdClicked() {
    qDebug() << "handleDeleteIdClicked";

    int nID = ui->id->value();

    int result = QMessageBox::warning(0,
        "Confirmation",
        QString("Are you sure to delete ID %1?").arg(nID),
        QMessageBox::Ok | QMessageBox::Cancel
    );

    if (result == QMessageBox::Ok) {
        if(oem.deleteId(nID) < 0) {
            setResult("Communication error");
            return;
        }
        if (oem.gwLastAck == NACK_INFO) {
            setResult(oem.utilError(oem.gwLastAckParam, nID));
            return;
        }

        setResult("Delete OK!");
    } else if (result == QMessageBox::Cancel) {
        setResult("Canceled!");
    }
}

void Handler::handleIdentifyClicked() {
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
    handlerUiPolling();

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

void Handler::handleDeleteAllClicked() {
    qDebug() << "handleDeleteAllClicked";

    int result = QMessageBox::warning(0,
        "Confirmation",
        QString("Are you sure to delete all?"),
        QMessageBox::Ok | QMessageBox::Cancel
    );

    if (result == QMessageBox::Ok) {
        if(oem.deleteAll() < 0) {
            setResult("Communication error");
            return;
        }
        if (oem.gwLastAck == NACK_INFO) {
            setResult(oem.utilError(oem.gwLastAckParam, 0));
            return;
        }

        setResult("Delete all OK!");
    } else if (result == QMessageBox::Cancel) {
        setResult("Canceled!");
    }
}

void Handler::handleVerifyTemplateClicked() {
    qDebug() << "handleVerifyTemplateClicked";

    int nID = ui->id->value();

    if(oem.checkEnrolled(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(oem.utilError(oem.gwLastAckParam, nID));
        return;
    }

    QString m_strFilePath = QFileDialog::getOpenFileName(0,
         tr("Open Template File"), tr("."), tr("Template files(*.dat *.bin *.db)"));

    if(m_strFilePath.isEmpty()) {
        return;
    }

    qint64 ds = QDateTime::currentMSecsSinceEpoch();

    QFile m_File(m_strFilePath);
    if (!m_File.open(QIODevice::ReadOnly)) {
        setResult("Cannot open file for read!");
        return;
    }

    // Read the file bit by bit until the end of it.
    int currentRead = 0;
    while(currentRead < FP_TEMPLATE_SIZE) {
        int r = m_File.read((char*)oem.gbyTemplate+currentRead, FP_TEMPLATE_SIZE-currentRead);
        if (r < 0) {
            break;
        }
        currentRead += r;
    }

    m_File.close();

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

void Handler::handleGetTemplateClicked() {
    qDebug() << "handleGetTemplateClicked";

    int nID = ui->id->value();

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

    QString m_strFilePath = QFileDialog::getSaveFileName(
        0,
        tr("Template File"),
        tr("."),
        tr("Template Files (*.bin *.db *.dat)")
    );
    if(m_strFilePath.isEmpty()) {
        setResult("Canceled!");
        return;
    }

    QFile m_File(m_strFilePath);

    if(!m_File.open(QIODevice::WriteOnly)) {
        setResult("Cannot create file for write!");
        return;
    }


    qint64 written = 0;
    while (written < FP_TEMPLATE_SIZE) {
        int w = m_File.write((char*)(oem.gbyTemplate+written), FP_TEMPLATE_SIZE-written);
        if (w < 0){
            break;
        }
        written += w;
    }

    m_File.close();

    if (written != FP_TEMPLATE_SIZE) {
        setResult("Error while writing file!");
        return;
    }

    //////////////////////////////////////////////////////////////////////////

    setResult(QString("Get Template OK (ID = %1)!").arg(nID));
}

void Handler::handleIdentifyTemplateClicked() {
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

    QString m_strFilePath = QFileDialog::getOpenFileName(
        0,
        tr("Template File"),
        tr("."),
        tr("Template Files (*.db *.dat *.bin)")
    );
    if(m_strFilePath.isEmpty()) {
        setResult("Action canceled!");
        return;
    }

    qint64 ds = QDateTime::currentMSecsSinceEpoch();

    QFile m_File;
    if(!m_File.open(QIODevice::ReadOnly)) {
        setResult("Cannot open file for read!");
        return;
    }

    int nLength = m_File.size();
    if(nLength != FP_TEMPLATE_SIZE) {
        setResult("Invalid size for read!");
        return;
    }

    int read = 0;
    while (read < FP_TEMPLATE_SIZE) {
        qint64 r = m_File.read((char*)(oem.gbyTemplate+read), FP_TEMPLATE_SIZE-read);
        if (r < 0) {
            break;
        }
        read += r;

    }
    m_File.close();

    if (read != FP_TEMPLATE_SIZE) {
        setResult("Error while reading the template file");
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

void Handler::handleSetTemplateClicked() {
    qDebug() << "handleSetTemplateClicked";

    int nID = ui->id->value();

        QString m_strFilePath = QFileDialog::getOpenFileName(
            0,
            tr("Template File"),
            tr("."),
            tr("Template Files (*.db *.bin *.dat)")
        );
        if(m_strFilePath.isEmpty()) {
            setResult("Setting template canceled!");
            return;
        }

    QFile m_File(m_strFilePath);
    if(!m_File.open(QIODevice::ReadOnly)) {
        setResult("Cannot open file for read!");
        return;
    }

    int nLength = m_File.size();
    if(nLength != FP_TEMPLATE_SIZE ) {
        setResult("Invalid size for read!");
        return;
    }

    int read = 0;
    while(read < FP_TEMPLATE_SIZE) {
        qint64 r = m_File.read((char*)(oem.gbyTemplate+read), FP_TEMPLATE_SIZE-read);
        if (r < 0) {
            break;
        }
        read += r;
    }
    m_File.close();

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

void Handler::handleIsPressedFingerClicked() {
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

void Handler::handleGetDatabaseClicked() {
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
        handlerUiPolling();

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
    QString m_strFilePath = QFileDialog::getSaveFileName(
        0,
        tr("Database File"),
        tr("."),
        tr("Database Files (*.dat *.bin *.db)")
    );

    if(m_strFilePath.isEmpty()) {
        setResult("File save canceled!");
        return;
    }

    QFile m_File(m_strFilePath);
    if(!m_File.open(QIODevice::WriteOnly)) {
        setResult("Cannot create file for write!");
        return;
    }

    qint64 written = 0;
    while(written < FP_TEMPLATE_DB_SIZE) {
        qint64 w = m_File.write((char*)(oem.gbyTemplateDB+written), FP_TEMPLATE_DB_SIZE-written);
        if (w < 0) {
            break;
        }
        written += w;
    }

    m_File.close();

    if (written != FP_TEMPLATE_DB_SIZE) {
        setResult("Error while saving the file!");
        return;
    }

    //////////////////////////////////////////////////////////////////////////

    setResult(QString("Get Database OK (Enroll count = %1)!").arg(nEnrollCount));
}

void Handler::handleGetImageClicked() {
    qDebug() << "handleGetImageClicked";

    LedLocker ll;
    Q_UNUSED(&ll);

    setResult("Input finger !");
    handlerUiPolling();

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

void Handler::handleSetDatabaseClicked() {
    qDebug() << "handleSetDatabaseClicked";

    // Ask for a database filename!
    QString m_strFilePath = QFileDialog::getOpenFileName(
        0,
        tr("Database File"),
        tr("."),
        tr("Database Files (*.db *.bin *.dat)")
    );
    if(m_strFilePath.isEmpty()) {
        return;
    }

    QFile m_File(m_strFilePath);
    if(!m_File.open(QIODevice::ReadOnly)) {
        setResult("Cannot open file for read!");
        return;
    }

    // Read the whole file!
    int nLength = m_File.size();
    int totalSize = FP_TEMPLATE_SIZE * (FP_MAX_USERS+1) + (FP_MAX_USERS+1);
    if(nLength != totalSize) {
        setResult("Invalid size for read!");
        return;
    }
    int read = 0;
    while(read < totalSize) {
        int r = m_File.read( (char*)(&oem.gbyTemplateDB[0] + read), totalSize-read);
        if (r < 0) {
            break;
        }
        read += r;
    }
    m_File.close();

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
        handlerUiPolling();

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

void Handler::handleGetRawImageClicked() {
    qDebug() << "handleGetRawImageClicked";

    LedLocker ll;
    Q_UNUSED(&ll);

    setResult("Input finger !");
    handlerUiPolling();

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

void Handler::handleCancelClicked() {
    qDebug() << "handleCancelClicked";

    bContinue = false;
}

void Handler::handleGetLiveImageClicked() {
    qDebug() << "handleGetLiveImageClicked";

    ui->disableOnLive();

    LedLocker ll;
    Q_UNUSED(&ll);

    bContinue = true;

    setResult("Downloading Live Image ...");
    handlerUiPolling();

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
