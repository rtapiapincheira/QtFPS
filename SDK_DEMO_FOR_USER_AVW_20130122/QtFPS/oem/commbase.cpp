#include "oem.h"

CCommSerial::CCommSerial() {
}

CCommSerial::~CCommSerial() {
    close();
}

bool CCommSerial::open(int nPort, uint dwBaudrate) {
    close();

    m_serialport.setPortName(QString("COM%1").arg(nPort));
    m_serialport.setBaudRate(dwBaudrate, QSerialPort::AllDirections);
    m_serialport.setDataBits(QSerialPort::Data8);
    m_serialport.setParity(QSerialPort::NoParity);
    m_serialport.setStopBits(QSerialPort::OneStop);

    return m_serialport.open(QIODevice::ReadWrite);
}

bool CCommSerial::close() {
    if (m_serialport.isOpen()) {
        m_serialport.close();
        return true;
    }
    return false;
}

int CCommSerial::sendData(uchar *buffer, uint nSize, uint dwTimeOut) {
    //qDebug() << "Sending data to open port," << nSize << "bytes";

    qint64 written = 0;
    while(written < nSize) {
        written += m_serialport.write((char*)(buffer+written), nSize-written);
        if ( ! m_serialport.waitForBytesWritten(dwTimeOut)) {
            qDebug() << "Writing operation timed out, error while communicating";
            break;
        }
    }
    //qDebug() << "written:" << written;
    return written;
}

int CCommSerial::recvData(uchar *buffer, uint nSize, uint dwTimeOut) {

    qDebug() << "trying to read" << nSize << "bytes from open port, dwTimeOut:" << dwTimeOut;

    qint64 ds = QDateTime::currentMSecsSinceEpoch();

    uint read = 0;
    while (read < nSize && (QDateTime::currentMSecsSinceEpoch() - ds < dwTimeOut)) {
        //qDebug() << "Waiting some bytes";

        // If no bytes, then wait
        if(!m_serialport.bytesAvailable()) {
            qApp->processEvents();
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
        }
    }

    qDebug() << "Total bytes read: " << read << "out of " << nSize;

    return read;
}

/************************************************************************/
/*       global function definitions                                    */
/************************************************************************/
CCommSerial comm_serial;

int comm_send(uchar* buffer, uint nsize, int ntimeout) {
    return comm_serial.sendData(buffer, nsize, ntimeout);
}

int comm_recv(uchar* buffer, uint nsize, int ntimeout) {
    return comm_serial.recvData(buffer, nsize, ntimeout);
}

bool comm_open_serial(int nComNumber, uint nComBaudRate) {
	comm_close();

    if(!comm_serial.open(nComNumber, nComBaudRate)) {
        qDebug() << "couldn't open, so closing it";
		comm_close();
        return false;
	}
	
    qDebug() << "Port open!";

    return true;
}

void comm_close() {
    comm_serial.close();
}
