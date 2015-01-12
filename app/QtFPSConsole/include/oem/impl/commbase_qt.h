#include "config.h"

#ifndef QT_COMM_SERIAL_H
#define QT_COMM_SERIAL_H

#ifdef QT_SERIAL_COMM

#include "commbase.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

/**
 * @brief The QtCommSerial class wraps the serial communication using the provided QtSerialPort
 * class which is available from Qt5.0 and above.
 *
 * This class implements the CCommSerial interface, see CCommSerial for more information on the
 * contract of this class.
 */
class QtCommSerial : public CCommSerial {

    QSerialPort m_serialport;

public:
    QtCommSerial();

    virtual ~QtCommSerial();

    virtual bool open(const std::string &port, uint dwBaudrate);

    virtual bool close();

    virtual qint64 write(uchar *buffer, uint nSize);

    virtual qint64 read(uchar *buffer, uint nSize);
};

#endif // QT_SERIAL_COMM

#endif // QT_COMM_SERIAL_H
