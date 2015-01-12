#include "config.h"

#ifdef RS232_SERIAL_COMM

#include "commbase_rs232.h"

Rs232CommSerial::Rs232CommSerial() :
    lastOpen(-1)
{
}

Rs232CommSerial::~Rs232CommSerial() {
    close();
}

bool Rs232CommSerial::open(const QString &port, uint dwBaudrate) {
    if (lastOpen != -1) {
#ifdef OUTPUT_DEBUG
        qDebug() << "Port already open, closing it first!";
#endif
        close();
    }

    int r = RS232_OpenComport(port.toLocal8Bit().constData(), SINGLETON_PORT_ID, dwBaudrate, "8N1");
    if (r == 0) {
#ifdef OUTPUT_DEBUG
        qDebug() << "Port open OK!";
#endif
        lastOpen = SINGLETON_PORT_ID;
        return true;
    }

    return false;
}

bool Rs232CommSerial::close() {
    if (lastOpen != -1) {
        RS232_CloseComport(lastOpen);
        lastOpen = -1;
        return true;
    }
    return false;
}

qint64 Rs232CommSerial::write(uchar *buffer, uint nSize) {
#ifdef OUTPUT_DEBUG
    qDebug() << "Sending data to open port," << nSize << "bytes";
#endif

    qint64 ds = QDateTime::currentMSecsSinceEpoch();
    const uint chunkSize = 512;

    uint written = 0;
    while(written < nSize && (QDateTime::currentMSecsSinceEpoch() - ds < timeOut)) {

        qint64 w = RS232_SendBuf(lastOpen, buffer+written, qMin(chunkSize, nSize-written));
        if (w < 0) {
            break;
        }
        written += w;

        if (m_listener) {
            (*m_listener)(m_parameter);
        }

        // TODO: maybe add a pause here to allow the reading from the other side
    }
#ifdef OUTPUT_DEBUG
    qDebug() << "written:" << written;
#endif
    return written;
}

qint64 Rs232CommSerial::read(uchar *buffer, uint nSize) {
#ifdef OUTPUT_DEBUG
    qDebug() << "trying to read" << nSize << "bytes from open port";
#endif

    qint64 ds = QDateTime::currentMSecsSinceEpoch();
    const uint chunkSize = 512;

    uint read = 0;
    while (read < nSize && (QDateTime::currentMSecsSinceEpoch() - ds < timeOut)) {
#ifdef OUTPUT_DEBUG
        qDebug() << "Waiting some bytes";
#endif

        int r = RS232_PollComport(lastOpen, buffer+read, qMin(chunkSize, nSize-read));
        if(r < 0) {
            break;
        }

        read += r;

        if (m_listener) {
            (*m_listener)(m_parameter);
        }

        // TODO: may add a pause here to allow the writing from the other side
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

#endif // RS232_SERIAL_COMM
