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

bool CCommSerial::open(const QString &port, uint dwBaudrate) {
    if(m_serialport.isOpen()) {
        m_serialport.close();
#ifdef OUTPUT_DEBUG
        qDebug() << "CComSerial::open already open, closing first";
#endif
    }

    m_serialport.setPortName(port);
    m_serialport.setBaudRate(dwBaudrate, QSerialPort::AllDirections);
    m_serialport.setDataBits(QSerialPort::Data8);
    m_serialport.setParity(QSerialPort::NoParity);
    m_serialport.setStopBits(QSerialPort::OneStop);

    bool b = m_serialport.open(QIODevice::ReadWrite);
#ifdef OUTPUT_DEBUG
    qDebug() << "CComSerial::open returning" << b;
#endif
    return b;
}

bool CCommSerial::close() {
    if (m_serialport.isOpen()) {
        m_serialport.close();
        return true;
    }
    return false;
}

qint64 CCommSerial::write(uchar *buffer, uint nSize) {
#ifdef OUTPUT_DEBUG
    qDebug() << "Sending data to open port," << nSize << "bytes";
#endif

    qint64 written = 0;
    while(written < nSize) {
        written += m_serialport.write((char*)(buffer+written), nSize-written);
        if (!m_serialport.waitForBytesWritten(timeOut)) {
#ifdef OUTPUT_DEBUG
            qDebug() << "Writing operation timed out, error while communicating";
#endif
            break;
        }
        if (m_listener) {
            (*m_listener)(m_parameter);
        }
    }
#ifdef OUTPUT_DEBUG
    qDebug() << "written:" << written;
#endif
    return written;
}

qint64 CCommSerial::read(uchar *buffer, uint nSize) {
#ifdef OUTPUT_DEBUG
    qDebug() << "trying to read" << nSize << "bytes from open port";
#endif

    qint64 ds = QDateTime::currentMSecsSinceEpoch();

    uint read = 0;
    while (read < nSize && (QDateTime::currentMSecsSinceEpoch() - ds < timeOut)) {
#ifdef OUTPUT_DEBUG
        qDebug() << "Waiting some bytes";
#endif

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
#ifdef OUTPUT_DEBUG
            qDebug() << "reading at most" << nSize-read << "bytes";
#endif
            int r = m_serialport.read((char*) (buffer+read), nSize-read);
            read += r;
#ifdef OUTPUT_DEBUG
            qDebug() << "    ->> read" << r << "bytes from port, " << read << "/" << nSize;
#endif
            if (m_listener) {
                (*m_listener)(m_parameter);
            }
        }
    }

#ifdef OUTPUT_DEBUG
    qDebug() << "Total bytes read: " << read << "out of " << nSize;
    QString g = "";
    for (int i = 0; i < read; i++) {
        g += " ";
        g += QString::number((uchar)buffer[i]);
    }
    qDebug() << "{" << g << "}";
#endif

    return read;
}
