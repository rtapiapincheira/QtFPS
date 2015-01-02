#ifndef __COMM_BASE_H_
#define __COMM_BASE_H_

#include <QtCore>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

int comm_send(uchar* pbuf, uint nsize, int ntimeout);
int comm_recv(uchar* pbuf, uint nsize, int ntimeout);
bool comm_open_serial(int nComNumber, uint nComBaudRate);
void comm_close();

class CCommSerial
{
public:
    CCommSerial();
    virtual ~CCommSerial();

    bool open(int nPort, uint dwBaudrate);
    bool close();

    int sendData(uchar *buffer, uint nSize, uint dwTimeOut);
    int recvData(uchar *buffer, uint nSize, uint dwTimeOut);

private:
    QSerialPort m_serialport;
};

#endif
