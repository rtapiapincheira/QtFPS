#ifndef __COMM_BASE_H_
#define __COMM_BASE_H_

#include <QtCore>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class CCommSerial {

private:
    QSerialPort m_serialport;
    uint timeOut;

    void (*m_listener)(void* param);
    void *m_parameter;

public:
    CCommSerial();
    virtual ~CCommSerial();

    void setCallback(void(*listener)(void*p), void *parameter);
    void setTimeout(uint dwTimeOut);

    bool open(int nPort, uint dwBaudrate);
    bool close();

    int write(uchar *buffer, uint nSize);
    int read(uchar *buffer, uint nSize);
};

#endif // __COMM_BASE_H_
