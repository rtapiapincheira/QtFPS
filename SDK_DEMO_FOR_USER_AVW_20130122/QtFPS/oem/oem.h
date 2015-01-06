#ifndef __OEM_H_
#define __OEM_H_

#include "commbase.h"
#include "sbprotocoloem.h"

/**
 * All the different possible commands to be senf to the fingerpring, as stated in its datasheet.
 */
enum {
    CMD_NONE                = 0x00,
    CMD_OPEN                = 0x01,
    CMD_CLOSE               = 0x02,
    CMD_USB_INTERNAL_CHECK  = 0x03,
    CMD_CHANGE_BAUDRATE     = 0x04,

    CMD_CMOS_LED            = 0x12,

    CMD_ENROLL_COUNT        = 0x20,
    CMD_CHECK_ENROLLED      = 0x21,
    CMD_ENROLL_START        = 0x22,
    CMD_ENROLL1             = 0x23,
    CMD_ENROLL2             = 0x24,
    CMD_ENROLL3             = 0x25,
    CMD_IS_PRESS_FINGER     = 0x26,

    CMD_DELETE              = 0x40,
    CMD_DELETE_ALL          = 0x41,

    CMD_VERIFY              = 0x50,
    CMD_IDENTIFY            = 0x51,
    CMD_VERIFY_TEMPLATE     = 0x52,
    CMD_IDENTIFY_TEMPLATE   = 0x53,

    CMD_CAPTURE             = 0x60,

    CMD_GET_IMAGE           = 0x62,
    CMD_GET_RAWIMAGE        = 0x63,

    CMD_GET_TEMPLATE        = 0x70,
    CMD_ADD_TEMPLATE        = 0x71,
    CMD_GET_DATABASE_START  = 0x72,
    CMD_GET_DATABASE_END    = 0x73,

    CMD_FW_UPDATE           = 0x80,
    CMD_ISO_UPDATE          = 0x81,

    ACK_OK                  = 0x30,
    NACK_INFO               = 0x31,
};

/**
 * All different possible acknowledge responses to be received from the fingerprint sensor, as
 * stated by its datasheed.
 */
enum {
        NACK_NONE                = 0x1000,
        NACK_TIMEOUT,
        NACK_INVALID_BAUDRATE,
        NACK_INVALID_POS,
        NACK_IS_NOT_USED,
        NACK_IS_ALREADY_USED,
        NACK_COMM_ERR,
        NACK_VERIFY_FAILED,
        NACK_IDENTIFY_FAILED,
        NACK_DB_IS_FULL,
        NACK_DB_IS_EMPTY,
        NACK_TURN_ERR,
        NACK_BAD_FINGER,
        NACK_ENROLL_FAILED,
        NACK_IS_NOT_SUPPORTED,
        NACK_DEV_ERR,
        NACK_CAPTURE_CANCELED,
        NACK_INVALID_PARAM,
        NACK_FINGER_IS_NOT_PRESSED,
};

/**
 * Handy enum for passing erros at application level (the fingerprint does not use this enum).
 */
enum {
        OEM_NONE                    = -2000,
        OEM_COMM_ERR,
};

/**
  * Holds some the information of the fingerprint sensor.
  */
typedef struct _devinfo {
    uint FirmwareVersion;
    uint IsoAreaMaxSize;
    uchar DeviceSerialNumber[16];
}
// TODO: add portable macros for specifying the alignment of structures.
__attribute__((packed))
devinfo;

#define FP_MAX_USERS        200
#define FP_TEMPLATE_SIZE    498
#define EEPROM_SIZE         16
#define FP_TEMPLATE_DB_SIZE (FP_TEMPLATE_SIZE * (FP_MAX_USERS+1) + (FP_MAX_USERS+1))

#define IMG8BIT_SIZE       256*256

/**
 * @brief The Oem class implements the high level communication with the fingerprint sensor, using
 * its communication protocol (see Oemp class).
 */
class Oem {

public:
    uchar    gbyImg8bit[IMG8BIT_SIZE];          // Image data of a pre-processed image taken from the sensor (smaller but unaccurate)
    uchar    gbyImgRaw[320*240];                // Image data of a raw image taken from the sensor           (bigger but accurate)
    uchar    gbyTemplate[FP_TEMPLATE_SIZE];     // Holds the template (simplified fingerprint data) of just one fingerprint
    uchar    gbyTemplateDB[FP_TEMPLATE_DB_SIZE];// Holds the templates of every fingerprint (200)

    ushort  gwDevID;        // Id of the fingerprint sensor (defaults to 1)
    ushort  gwLastAck;      // Acnowledge response from the commucation protocol
    int     gwLastAckParam; // Additional data for the acnowledge response
    devinfo gDevInfo;       // Fingerprint factory information

private:
    /**
     * @brief oemp implements the low level communication protocol (sends and reads packets of
     * information using protocol specified in the datasheet).
     */
    Oemp oemp;

    void (*m_listener)(void* param);
    void *m_parameter;

    int commandRun(ushort wCmd, int nCmdParam);

public:
    Oem();

    void setCallback(void(*listener)(void*p), void *parameter);

    int openPort(uint port, uint baudrate);
    int closePort();

    int openOem();
    int closeOem();

    int usbInternalCheck();
    int changeBaudrate(int nBaudrate);

    int cmosLed(bool bOn);

    int enrollCount();
    int checkEnrolled(int nPos);
    int enrollStart(int nPos);
    /*AVW*/
    int enrollNth(int nPos, int nTurn);
    int isPressFinger();

    int deleteId(int nPos);
    int deleteAll();

    int verify(int nPos);
    int identify();
    int verifyTemplate(int nPos);
    int identifyTemplate();

    int capture(bool bBest);
    int getImage();
    int getRawimage();

    int getTemplate(int nPos );
    int addTemplate(int nPos);
    int getDatabaseStart();
    int getDatabaseEnd();

//    int fwUpgrade(uchar* pBuf, int nLen);
//    int isoUpgrade(uchar* pBuf, int nLen);

public:
    // TODO: remove from here and put in an utilitary class for GUIs
    QString utilError(int nack, int nPos);
    int utilCapturing(bool best, QString &err);
    int utilDownloadingImage(QString &err);
    int utilLoadingImage(QString &err);
    int utilLoadingImageRaw(QString &err);
    int utilLoadingImageLive(QString &err);
};

#endif
