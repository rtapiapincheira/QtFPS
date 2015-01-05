#include "oem.h"

CCommSerial::CCommSerial() {
}

CCommSerial::~CCommSerial() {
    close();
}

void CCommSerial::setCallback(void(*listener)(void*p), void *parameter) {
    m_listener = listener;
    m_parameter = parameter;
}

void CCommSerial::setTimeout(uint dwTimeOut) {
    timeOut = dwTimeOut;
}

bool CCommSerial::open(int nPort, uint dwBaudrate) {
    if(m_serialport.isOpen()) {
        m_serialport.close();
        qDebug() << "CComSerial::open already open, closing first";
    }

    m_serialport.setPortName(QString("COM%1").arg(nPort));
    m_serialport.setBaudRate(dwBaudrate, QSerialPort::AllDirections);
    m_serialport.setDataBits(QSerialPort::Data8);
    m_serialport.setParity(QSerialPort::NoParity);
    m_serialport.setStopBits(QSerialPort::OneStop);

    bool b = m_serialport.open(QIODevice::ReadWrite);
    qDebug() << "CComSerial::open returning" << b;
    return b;
}

bool CCommSerial::close() {
    if (m_serialport.isOpen()) {
        m_serialport.close();
        return true;
    }
    return false;
}

int CCommSerial::write(uchar *buffer, uint nSize) {
    //qDebug() << "Sending data to open port," << nSize << "bytes";

    qint64 written = 0;
    while(written < nSize) {
        written += m_serialport.write((char*)(buffer+written), nSize-written);
        if (!m_serialport.waitForBytesWritten(timeOut)) {
            qDebug() << "Writing operation timed out, error while communicating";
            break;
        }
        if (m_listener) {
            (*m_listener)(m_parameter);
        }
    }
    //qDebug() << "written:" << written;
    return written;
}

int CCommSerial::read(uchar *buffer, uint nSize) {

    qDebug() << "trying to read" << nSize << "bytes from open port";

    qint64 ds = QDateTime::currentMSecsSinceEpoch();

    uint read = 0;
    while (read < nSize && (QDateTime::currentMSecsSinceEpoch() - ds < timeOut)) {
        //qDebug() << "Waiting some bytes";

        // If no bytes, then wait
        if(!m_serialport.bytesAvailable()) {
            if (m_listener) {
                (*m_listener)(m_parameter);
            }
            continue;
            /*
            // If the wait is timed out
            if(!m_serialport.waitForReadyRead(dwTimeOut)) {
                qDebug() << "Error, timed out while reading from communications, read:" << read;
                return read;
            }
            */
        }

        if (m_serialport.bytesAvailable() > 0 && read < nSize) {
            //qDebug() << "reading at most" << nSize-read << "bytes";
            int r = m_serialport.read((char*) (buffer+read), nSize-read);
            read += r;
            //qDebug() << "    ->> read" << r << "bytes from port, " << read << "/" << nSize;
            if (m_listener) {
                (*m_listener)(m_parameter);
            }
        }
    }

    qDebug() << "Total bytes read: " << read << "out of " << nSize;

    /*
    QString g = "";
    for (int i = 0; i < read; i++) {
        g += " ";
        g += QString::number((uchar)buffer[i]);
    }
    qDebug() << "{" << g << "}";
    */

    return read;
}
