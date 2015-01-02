#include "uihandler.h"

#include "oem/oem.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

LedLocker::LedLocker() {
    oem_cmos_led(true);
}

LedLocker::~LedLocker() {
    oem_cmos_led(false);
}

UiHandler::UiHandler(QObject *parent) :
    QObject(parent)
{
}

void UiHandler::setResult(const QString &line1, const QString &line2) {
    QString result = line1 + "\n" + line2;
    qDebug() << "setResult:" << result;
    ui->result->setText(result);
}

void UiHandler::uiPolling() {
    qApp->processEvents();
}

void UiHandler::setup(UiHelper *_ui) {
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

void UiHandler::handleSaveImageToFileClicked() {
    qDebug() << "handleSaveImageToFile";
}

void UiHandler::handleOpenClicked() {
    qDebug() << "handleOpenClicked";

    int port = ui->serialPortNumber->currentText().replace("COM","").toInt();

    qDebug() << "Opening port:" << port;

    int baud = ui->baudrate->currentText().toInt();

    if(!comm_open_serial(port, 9600) ||
        oem_change_baudrate(baud) < 0 ||
        !comm_open_serial(port, baud)){
        qDebug() << "Device is not connected to serial port.";
        return;
    }

    if(oem_open() < 0) {
        qDebug() << "Cannot connect to the device !";
        return;
    } if(gwLastAck == NACK_INFO) {
        qDebug() << "Cannot connect to the device !";
        return;
    }

    uchar* p = gDevInfo.DeviceSerialNumber;

    QString sn = "";
    for(int i = 0; i < 16; i++) {
        sn += (p[i] < 16 ? "0" : "") + QString::number(p[i], 16);
        if(i == 7) {
            sn += "-";
        }
    }

    setResult(
        QString("FirmwareVersion: %1, IsoAreaMaxSize: %2 KB\r\nDeviceSN: %3")
            .arg(QString::number(gDevInfo.FirmwareVersion, 16))
            .arg(gDevInfo.IsoAreaMaxSize / 1024)
            .arg(sn.toUpper())
    );

    ui->disableOnConnected();
}

void UiHandler::handleCloseClicked() {
    qDebug() << "handleCloseClicked";

    oem_change_baudrate(9600);
    comm_close();

    setResult("");

    ui->enableOnDisconnected();
}

void UiHandler::handleEnrollClicked() {
    qDebug() << "handleEnrollClicked";

    LedLocker ll;
    Q_UNUSED(&ll);

    int nID = ui->id->value();

    if(oem_enroll_start(nID) < 0) {
        setResult("Communication error!");
        return;
    }

    uiPolling();

    if(gwLastAck == NACK_INFO) {
        qDebug() << "handleEnrollClicked: error";
        setResult(my_oem_error(gwLastAckParam, nID));
        return;
    }

    for(int i=1; i<=3; i++) {
        setResult(QString("Input finger %1!").arg(i));
        uiPolling();

        QString err;
        if(my_oem_capturing(true, err) < 0) {
            setResult(err);
            return;
        }

        qint64 ds = QDateTime::currentMSecsSinceEpoch();

        setResult("Processing fingerprint...");
        uiPolling();

        if(oem_enroll_nth(nID, i) < 0) {
            setResult("Communication error!");
            return;
        }
        if(gwLastAck == NACK_INFO) {
            qDebug() << "this point!";
            setResult(my_oem_error(gwLastAckParam, 0));
            return;
        }

        qint64 dt = (QDateTime::currentMSecsSinceEpoch() - ds);

        setResult("Take off finger, please ...");
        uiPolling();

        if(i<3) {
            while(true) {
                if(oem_is_press_finger() < 0){
                    setResult("Communication error!");
                    return;
                }
                if(gwLastAck == NACK_INFO) {
                    qDebug() << "this other point!";
                    setResult(my_oem_error(gwLastAckParam, 0));
                    return;
                }
                if(gwLastAckParam != 0) {
                    break;
                }
            }
        }
        setResult(QString("Process time: %1 ms").arg(dt));
        uiPolling();
    }

    setResult(QString("Enroll OK (ID = %1)!").arg(nID));
    uiPolling();
}

void UiHandler::handleGetUserCountClicked() {
    qDebug() << "handleGetUserCountClicked";

    if(oem_enroll_count() < 0) {
        setResult("Communication error!");
        return;
    }
    if(gwLastAck == NACK_INFO) {
        setResult(my_oem_error(gwLastAckParam, 0));
        return;
    }

    setResult(QString("Enroll count = %1 !").arg(gwLastAckParam));
}

void UiHandler::handleVerifyClicked() {
    qDebug() << "handleVerifyClicked";

    LedLocker ll;
    Q_UNUSED(&ll);

    int nID = ui->id->value();

    if(oem_check_enrolled(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(gwLastAck == NACK_INFO) {
        setResult(my_oem_error(gwLastAckParam, nID));
        return;
    }

    setResult("");
    uiPolling();

    QString err;

    while(true) {
        setResult("Input finger !");
        uiPolling();

        qint64 ds = QDateTime::currentMSecsSinceEpoch();

        if(my_oem_capturing(false, err) < 0) {
            qDebug() << "my_oem_capturing, err:" << err;
            return;
        }

        setResult("Processing fingerprint...");
        uiPolling();

        if(oem_verify(nID) < 0) {
            setResult("Communication error!");
            return;
        }

        if(gwLastAck == NACK_INFO) {
            setResult(my_oem_error(gwLastAckParam, nID));
            continue;
        }

        setResult(QString("ID = %1      : %2 ms").arg(nID).arg(QDateTime::currentMSecsSinceEpoch() - ds));
        break;
    }
}

void UiHandler::handleDeleteIdClicked() {
    qDebug() << "handleDeleteIdClicked";

    int nID = ui->id->value();

    int result = QMessageBox::warning(0,
        "Confirmation",
        QString("Are you sure to delete ID %1?").arg(nID),
        QMessageBox::Ok | QMessageBox::Cancel
    );

    if (result == QMessageBox::Ok) {
        if(oem_delete(nID) < 0) {
            setResult("Communication error");
            return;
        }
        if (gwLastAck == NACK_INFO) {
            setResult(my_oem_error(gwLastAckParam, nID));
            return;
        }

        setResult("Delete OK!");
    } else if (result == QMessageBox::Cancel) {
        setResult("Canceled!");
    }
}

void UiHandler::handleIdentifyClicked() {
    qDebug() << "handleIdentifyClicked";
}

void UiHandler::handleDeleteAllClicked() {
    qDebug() << "handleDeleteAllClicked";

    int result = QMessageBox::warning(0,
        "Confirmation",
        QString("Are you sure to delete all?"),
        QMessageBox::Ok | QMessageBox::Cancel
    );

    if (result == QMessageBox::Ok) {
        if(oem_delete_all() < 0) {
            setResult("Communication error");
            return;
        }
        if (gwLastAck == NACK_INFO) {
            setResult(my_oem_error(gwLastAckParam, 0));
            return;
        }

        setResult("Delete all OK!");
    } else if (result == QMessageBox::Cancel) {
        setResult("Canceled!");
    }
}

void UiHandler::handleVerifyTemplateClicked() {
    qDebug() << "handleVerifyTemplateClicked";

    int nID = ui->id->value();

    if(oem_check_enrolled(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(gwLastAck == NACK_INFO) {
        setResult(my_oem_error(gwLastAckParam, nID));
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
        int r = m_File.read((char*)gbyTemplate+currentRead, FP_TEMPLATE_SIZE-currentRead);
        if (r <= 0) {
            break;
        }
        currentRead += r;
    }

    m_File.close();

    if(currentRead != FP_TEMPLATE_SIZE) {
        setResult("Invalid size for read!");
        return;
    }

    if(oem_verify_template(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(gwLastAck == NACK_INFO) {
        setResult(my_oem_error(gwLastAck, nID));
        return;
    }

    setResult(QString("ID = %1      : %2 ms").arg(nID).arg(QDateTime::currentMSecsSinceEpoch()-ds));
}

void UiHandler::handleGetTemplateClicked() {
    qDebug() << "handleGetTemplateClicked";
}

void UiHandler::handleIdentifyTemplateClicked() {
    qDebug() << "handleIdentifyTemplateClicked";
}

void UiHandler::handleSetTemplateClicked() {
    qDebug() << "handleSetTemplateClicked";
}

void UiHandler::handleIsPressedFingerClicked() {
    qDebug() << "handleIsPressedFingerClicked";

    LedLocker ll;
    Q_UNUSED(&ll);

    if(oem_is_press_finger() < 0){
        setResult("Communication error");
        return;
    }
    if(gwLastAck == NACK_INFO){
        setResult(my_oem_error(gwLastAckParam, 0));
        return;
    }
    if(gwLastAckParam != 0) {
        setResult("Finger is not pressed!");
        return;
    }

    setResult("Finger is pressed!");
}

void UiHandler::handleGetDatabaseClicked() {
    qDebug() << "handleGetDatabaseClicked";
}

void UiHandler::handleGetImageClicked() {
    qDebug() << "handleGetImageClicked";

    LedLocker ll;
    Q_UNUSED(&ll);
    oem_cmos_led(true);

    setResult("Input finger !");
    uiPolling();

    QString err;
    if(my_oem_capturing(true, err) < 0) {
        qDebug() << "my_oem_capturing: error";
        setResult(err);
        return;
    }
    if(my_oem_loading_image(err) < 0) {
        qDebug() << "my_oem_loading_image: error";
        setResult(err);
        return;
    }

    ui->drawImage(gbyImg8bit, 0, 0, 256, 256);
    setResult("Get Image OK !");
}

void UiHandler::handleSetDatabaseClicked() {
    qDebug() << "handleSetDatabaseClicked";
}

void UiHandler::handleGetRawImageClicked() {
    qDebug() << "handleGetRawImageClicked";
}

void UiHandler::handleCancelClicked() {
    qDebug() << "handleCancelClicked";
}

void UiHandler::handleGetLiveImageClicked() {
    qDebug() << "handleGetLiveImageClicked";
}
