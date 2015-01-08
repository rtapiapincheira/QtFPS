#include "config.h"

#ifndef RS232_COMM_SERIAL_H
#define RS232_COMM_SERIAL_H

#ifdef RS232_SERIAL_COMM

#include "rs232.h"

#include "commbase.h"

#define SINGLETON_PORT_ID 1

class Rs232CommSerial : public CCommSerial {
private:

    int lastOpen;

public:
    Rs232CommSerial();

    virtual ~Rs232CommSerial();

    virtual bool open(const QString &port, uint dwBaudrate);

    virtual bool close();

    virtual qint64 write(uchar *buffer, uint nSize);

    virtual qint64 read(uchar *buffer, uint nSize);
};

#endif // RS232_SERIAL_COMM

#endif // RS232_COMM_SERIAL_H
