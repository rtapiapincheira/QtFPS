#include "config.h"

#ifndef QT_COMM_SERIAL_H
#define QT_COMM_SERIAL_H

#ifdef QT_SERIAL_COMM

#include "commbase.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class QtCommSerial : public CCommSerial {
    /**
     * @brief m_serialport specific mechanism for serial communication. Refer to the Qt
     * documentation for specific details on its usage.
     */
    QSerialPort m_serialport;

public:
    QtCommSerial();

    virtual ~QtCommSerial();

    virtual bool open(const QString &port, uint dwBaudrate);

    virtual bool close();

    virtual qint64 write(uchar *buffer, uint nSize);

    virtual qint64 read(uchar *buffer, uint nSize);
};

#endif // QT_SERIAL_COMM

#endif // QT_COMM_SERIAL_H
