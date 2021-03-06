#include "config.h"
#include "controller.h"
#include "oem.h"

#include <cstdio>
#include <cstring>

LedLocker::LedLocker(Oem *_oem) {
    oem = _oem;
    oem->cmosLed(true);
}

LedLocker::~LedLocker() {
    oem->cmosLed(false);
}

void handlerUiPolling(void *parameter) {
// Do nothing!
}

Controller::Controller(CCommSerial *serial_port) :
    oem(serial_port)
{
}

void Controller::setResult(const std::string &line1, const std::string &line2) {
    ui->setResult(line1, line2);
    handlerUiPolling(this);
}

void Controller::setup(Helper *_ui, void (*_handler)(void* param)) {
    ui = _ui;
    handlerUiPolling = _handler;

    oem.setCallback(_handler, this);
}

void Controller::__saveImageToFile() {
    std::string filename = ui->getSaveFilename(EXTENSION_IMAGE, "pic.png");
    if(filename.empty()) {
        setResult("Save image canceled!");
        return;
    }

    ui->saveLastImage(filename);
}

void Controller::__open() {
    std::string port = ui->getComPort();

#ifdef OUTPUT_DEBUG
    printf("Opening port: %s\n", port.c_str());
#endif

    uint baud = ui->getComBaudrate();

#ifdef OUTPUT_DEBUG
    printf("Using baudrate: %d\n", baud);
#endif

    if(oem.openPort(port, baud) < 0){
#ifdef OUTPUT_DEBUG
        printf("Device is not connected to serial port.");
#endif
        return;
    }

    if(oem.openOem() < 0) {
#ifdef OUTPUT_DEBUG
        printf("Cannot connect to the device!");
#endif
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
#ifdef OUTPUT_DEBUG
        printf("Cannot connect to the device!");
#endif
        return;
    }
}

void Controller::__close() {
    oem.closePort();
}

void Controller::__enroll() {
    LedLocker ll(&oem);

    int nID = ui->getId();

    if(oem.enrollStart(nID) < 0) {
        setResult("Communication error!");
        return;
    }

    if(oem.gwLastAck == NACK_INFO) {
#ifdef OUTPUT_DEBUG
        printf("handleEnrollClicked: error\n");
#endif
        setResult(Oem::error(oem.gwLastAckParam, nID));
        return;
    }

    for(int i=1; i<=3; i++) {
        setResult(Utils::format("Input finger %d!", i));

        std::string err;
        if(oem.capturing(true, err) < 0) {
            setResult(err);
            return;
        }

        int64 ds = Utils::currentMillis();

        setResult("Processing fingerprint...");

        if(oem.enrollNth(nID, i) < 0) {
            setResult("Communication error!");
            return;
        }
        if(oem.gwLastAck == NACK_INFO) {
#ifdef OUTPUT_DEBUG
            printf("NACK_INFO error #1\n");
#endif
            setResult(Oem::error(oem.gwLastAckParam, 0));
            return;
        }

        int64 dt = Utils::currentMillis() - ds;

        setResult("Take off finger, please ...");

        if(i<3) {
            while(true) {
                if(oem.isPressFinger() < 0){
                    setResult("Communication error!");
                    return;
                }
                if(oem.gwLastAck == NACK_INFO) {
#ifdef OUTPUT_DEBUG
                    printf("NACK_INFO error #2\n");
#endif
                    setResult(Oem::error(oem.gwLastAckParam, 0));
                    return;
                }
                if(oem.gwLastAckParam!= 0) {
                    break;
                }
            }
        }
        setResult(Utils::format("Process time: %d ms", dt));
    }

    setResult(Utils::format("Enroll OK (ID = %d)!", nID));
}

void Controller::__getUserCount() {
    if(oem.enrollCount() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(Oem::error(oem.gwLastAckParam, 0));
        return;
    }

    setResult(Utils::format("Enroll count = %d!", oem.gwLastAckParam));
}

void Controller::__verify() {
    int nID = ui->getId();

    if(oem.checkEnrolled(nID) < 0) {
        setResult("Communication error!");
        return;
    }

    if(oem.gwLastAck == NACK_INFO) {
        setResult(Oem::error(oem.gwLastAckParam, nID));
        return;
    }

    LedLocker ll(&oem);

    setResult("Input finger!");

    int64 ds = Utils::currentMillis();

    std::string err;
    if(oem.capturing(false, err) < 0) {
#ifdef OUTPUT_DEBUG
        printf("my_oem_capturing, err: %s\n", err.c_str());
#endif
        return;
    }

    setResult("Processing fingerprint...");

    if(oem.verify(nID) < 0) {
        setResult("Communication error!");
        return;
    }

    if(oem.gwLastAck == NACK_INFO) {
        setResult(Oem::error(oem.gwLastAckParam, nID));
        return;
    }

    setResult(Utils::format("ID = %d (MATCH!) : %d ms", nID, Utils::currentMillis() - ds));
}

void Controller::__deleteId() {
    int nID = ui->getId();

    std::string message = Utils::format("Are you sure to delete ID %d?", nID);
    if (ui->confirm(message)) {
        if(oem.deleteId(nID) < 0) {
            setResult("Communication error");
            return;
        }
        if (oem.gwLastAck == NACK_INFO) {
            setResult(Oem::error(oem.gwLastAckParam, nID));
            return;
        }
        setResult("Delete OK!");
    } else {
        setResult("Canceled!");
    }
}

void Controller::__identify() {
    if(oem.enrollCount() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(Oem::error(oem.gwLastAckParam, 0));
        return;
    }
    if (oem.gwLastAckParam == 0) {
        setResult("No user!");
        return;
    }

    LedLocker ll(&oem);

    setResult("Input finger!");

    std::string err;
    if(oem.capturing(false, err) < 0) {
        setResult(err);
        return;
    }

    int64 ds = Utils::currentMillis();

    setResult("Processing fingerprint...");

    if(oem.identify() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(Oem::error(oem.gwLastAckParam, 0));
        return;
    }

    setResult(Utils::format("ID = %d      : %d ms", oem.gwLastAckParam, Utils::currentMillis() - ds));
}

void Controller::__deleteAll() {
    if (ui->confirm("Are you sure to delete all?")) {
        if(oem.deleteAll() < 0) {
            setResult("Communication error");
            return;
        }
        if (oem.gwLastAck == NACK_INFO) {
            setResult(Oem::error(oem.gwLastAckParam, 0));
            return;
        }

        setResult("Delete all OK!");
    } else {
        setResult("Canceled!");
    }
}

void Controller::__verifyTemplate() {
    int nID = ui->getId();

    if(oem.checkEnrolled(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(Oem::error(oem.gwLastAckParam, nID));
        return;
    }

    std::string filename = ui->getOpenFilename(EXTENSION_TEMPLATE);
    if(filename.empty()) {
        setResult("Open file canceled!");
        return;
    }

    int64 ds = Utils::currentMillis();

    int64 currentRead = Utils::readAll(filename, oem.gbyTemplate, FP_TEMPLATE_SIZE);
    if (currentRead < 0) {
        setResult("Cannot open file for read!");
        return;
    }

    if(currentRead!= FP_TEMPLATE_SIZE) {
        setResult("Invalid size for read!");
        return;
    }

    if(oem.verifyTemplate(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(Oem::error(oem.gwLastAck, nID));
        return;
    }

    setResult(Utils::format("ID = %d      : %d ms", nID, Utils::currentMillis()-ds));
}

void Controller::__getTemplate() {
    int nID = ui->getId();

    setResult(Utils::format("Getting of %dth template...", nID));

    if(oem.getTemplate(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck==NACK_INFO ) {
        setResult(Oem::error(oem.gwLastAckParam, nID));
        return;
    }

    std::string filename = ui->getSaveFilename(EXTENSION_TEMPLATE);
    if(filename.empty()) {
        setResult("Open file anceled!");
        return;
    }

    int64 written = Utils::writeAll(filename, oem.gbyTemplate, FP_TEMPLATE_SIZE);
    if (written < 0) {
        setResult("Cannot create file for write!");
        return;
    }
    if (written!= FP_TEMPLATE_SIZE) {
        setResult("Error while writing file!");
        return;
    }

    //////////////////////////////////////////////////////////////////////////

    setResult(Utils::format("Get Template OK (ID = %d)!", nID));
}

void Controller::__identifyTemplate() {
    if(oem.enrollCount() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(Oem::error(oem.gwLastAckParam, 0));
        return;
    }

    if(oem.gwLastAckParam == 0) {
        setResult("No user!");
        return;
    }

    std::string filename = ui->getOpenFilename(EXTENSION_TEMPLATE);
    if(filename.empty()) {
        setResult("Open file canceled!");
        return;
    }

    int64 ds = Utils::currentMillis();

    int64 currentRead = Utils::readAll(filename, oem.gbyTemplate, FP_TEMPLATE_SIZE);
    if(currentRead < 0) {
        setResult("Cannot open file for read!");
        return;
    }

    if(currentRead!= FP_TEMPLATE_SIZE) {
        setResult("Error while reading the file!");
        return;
    }

    if(oem.identifyTemplate() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(Oem::error(oem.gwLastAckParam, 0));
        return;
    }

    setResult(Utils::format("ID = %d      : %d ms", oem.gwLastAckParam, Utils::currentMillis() - ds));
}

void Controller::__setTemplate() {
    int nID = ui->getId();

    std::string filename = ui->getOpenFilename(EXTENSION_TEMPLATE);
    if(filename.empty()) {
        setResult("Setting template canceled!");
        return;
    }

    int64 currentRead = Utils::readAll(filename, oem.gbyTemplate, FP_TEMPLATE_SIZE);
    if(currentRead < 0) {
        setResult("Cannot open file for read!");
        return;
    }
    if(currentRead!= FP_TEMPLATE_SIZE ) {
        setResult("Invalid size for read!");
        return;
    }

    //////////////////////////////////////////////////////////////////////////

    setResult(Utils::format("Adding of %dth template...", nID));

    if(oem.addTemplate(nID) < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck==NACK_INFO) {
        setResult(Oem::error(oem.gwLastAckParam, nID));
        return;
    }

    setResult(Utils::format("Set Template OK (ID = %d)!", nID));
}

void Controller::__isPressedFinger() {
    LedLocker ll(&oem);

    if(oem.isPressFinger() < 0){
        setResult("Communication error");
        return;
    }
    if(oem.gwLastAck == NACK_INFO){
        setResult(Oem::error(oem.gwLastAckParam, 0));
        return;
    }
    if(oem.gwLastAckParam!= 0) {
        setResult("Finger is not pressed!");
        return;
    }

    setResult("Finger is pressed!");
}

void Controller::__getDatabase() {
    int nIndexBufPos = FP_TEMPLATE_SIZE * (FP_MAX_USERS+1);

    if(oem.enrollCount() < 0) {
        setResult("Communication error!");
        return;
    }
    if(oem.gwLastAck == NACK_INFO) {
        setResult(Oem::error(oem.gwLastAckParam, 0));
        return;
    }

    if (oem.gwLastAckParam == 0) {
        setResult("No user!");
        return;
    }

    if(oem.getDatabaseStart() < 0){
        setResult("Communication error!");
        return;
    }

    int nEnrollCount = 0;
    for (int i=0; i<FP_MAX_USERS; i++) {
        setResult(Utils::format("Getting of %dth template...", i));

        if(oem.getTemplate(i) < 0) {
             setResult("Communication error!");
             oem.getDatabaseEnd();
             return;
        }
        if(oem.gwLastAck==NACK_INFO && oem.gwLastAckParam!=NACK_IS_NOT_USED ) {
            setResult(Oem::error(oem.gwLastAckParam, i));
            oem.getDatabaseEnd();
            return;
        }

        if(oem.gwLastAck==NACK_INFO && oem.gwLastAckParam==NACK_IS_NOT_USED ) {
            uchar* addr = &oem.gbyTemplateDB[i*FP_TEMPLATE_SIZE];
            for(int k = 0; k < FP_TEMPLATE_SIZE; k++) {
                addr[k] = 0xFF;
            }
            oem.gbyTemplateDB[nIndexBufPos+i] = 0;
        } else {
            memcpy(&oem.gbyTemplateDB[i*FP_TEMPLATE_SIZE], oem.gbyTemplate, FP_TEMPLATE_SIZE);
            oem.gbyTemplateDB[nIndexBufPos+i] = 1;
            nEnrollCount++;
        }
    }
    oem.getDatabaseEnd();

    std::string filename = ui->getSaveFilename(EXTENSION_DATABASE);
    if(filename.empty()) {
        setResult("File save canceled!");
        return;
    }

    int64 currentWritten = Utils::writeAll(filename, oem.gbyTemplateDB, FP_TEMPLATE_DB_SIZE);
    if(currentWritten < 0) {
        setResult("Cannot create file for write!");
        return;
    }

    if (currentWritten != FP_TEMPLATE_DB_SIZE) {
        setResult("Error while saving the file!");
        return;
    }

    setResult(Utils::format("Get Database OK (Enroll count = %d)!", nEnrollCount));
}

void Controller::__getImage() {
    LedLocker ll(&oem);

    setResult("Input finger!");

    std::string err;
    if(oem.capturing(true, err) < 0) {
#ifdef OUTPUT_DEBUG
        printf("my_oem_capturing: error\n");
#endif
        setResult(err);
        return;
    }
    if(oem.loadingImage(err) < 0) {
#ifdef OUTPUT_DEBUG
        printf("my_oem_loading_image: error\n");
#endif
        setResult(err);
        return;
    }

    ui->setImage(oem.gbyImg8bit, Helper::Image256);
    setResult("Get Image OK!");
}

void Controller::__setDatabase() {
    // Ask for a database filename!
    std::string filename = ui->getOpenFilename(EXTENSION_DATABASE);
    if(filename.empty()) {
        return;
    }

    int64 currentRead = Utils::readAll(filename, oem.gbyTemplateDB, FP_TEMPLATE_DB_SIZE);
    if(currentRead < 0) {
        setResult("Cannot open file for read!");
        return;
    }

    // Read the whole file!
    if(currentRead!= FP_TEMPLATE_DB_SIZE) {
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
        setResult("No user!");
        return;
    }
    //////////////////////////////////////////////////////////////////////////

    int nAddCount = 0;
    for (int i=0; i<FP_MAX_USERS; i++) {

        if(oem.gbyTemplateDB[nIndexBufPos+i] == 0) {
            continue;
        }

        setResult(Utils::format("Adding of %dth template...", i));

        memcpy(oem.gbyTemplate, &oem.gbyTemplateDB[i*FP_TEMPLATE_SIZE], FP_TEMPLATE_SIZE);

        if(oem.addTemplate(i) < 0) {
            setResult("Communication error!");
            return;
        }
        if(oem.gwLastAck == NACK_INFO) {
            setResult(Oem::error(oem.gwLastAckParam, i));
            return;
        }
        nAddCount++;
    }

    setResult(Utils::format("Set Database OK (Enroll count = %d)!", nAddCount));
}

void Controller::__getRawImage() {
    LedLocker ll(&oem);

    setResult("Input finger!");

    std::string err;
    if(oem.capturing(true, err) < 0) {
#ifdef OUTPUT_DEBUG
        printf("my_oem_capturing: error\n");
#endif
        setResult(err);
        return;
    }
    if(oem.loadingImageRaw(err) < 0) {
#ifdef OUTPUT_DEBUG
        printf("my_oem_loading_image_raw: error\n");
#endif
        setResult(err);
        return;
    }

    ui->setImage(oem.gbyImgRaw, Helper::Image320);
    setResult("Get Image OK!");
}

void Controller::__cancel() {
    bContinue = false;
}

void Controller::__getLiveImage() {
    LedLocker ll(&oem);

    bContinue = true;

    setResult("Downloading Live Image ...");

    std::string err;

    while(bContinue) {
        if (oem.loadingImageLive(err) < 0) {
#ifdef OUTPUT_DEBUG
            printf("my_oem_loading_image_live: error\n");
#endif
            setResult(err);
            break;
        }
        handlerUiPolling(this);

        ui->setImage(oem.gbyImgRaw, Helper::Image320);
        handlerUiPolling(this);
    }

    setResult("Download of Live Image finished");
}
