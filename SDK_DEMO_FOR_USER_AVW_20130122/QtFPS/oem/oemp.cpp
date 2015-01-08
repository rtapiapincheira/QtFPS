#include "config.h"
#include "oemp.h"

#include "commbase.h"
#include <QDebug>

ushort Oemp::calcChkSumOfCmdAckPkt(SB_OEM_PKT* pPkt) {
    ushort wChkSum = 0;
    uchar* pBuf = (uchar*)pPkt;

    for (uint i=0; i<(sizeof(SB_OEM_PKT)-SB_OEM_CHK_SUM_SIZE); i++) {
        wChkSum += pBuf[i];
    }

    return wChkSum;
}

ushort Oemp::calcChkSumOfDataPkt(uchar* pDataPkt, int nSize) {
    ushort wChkSum = 0;
    uchar* pBuf = (uchar*)pDataPkt;

    for(int i=0; i<nSize; i++) {
        wChkSum += pBuf[i];
    }

    return wChkSum;
}

void Oemp::setCallback(void(*listener)(void*p), void *parameter) {
    serial_port.setCallback(listener, parameter);
}

int Oemp::open(const QString &port, uint baudrate) {
    if(serial_port.open(port, baudrate)) {
        serial_port.setTimeout(15000);
#ifdef OUTPUT_DEBUG
        qDebug() << "Returning 0";
#endif
        return 0;
    }
#ifdef OUTPUT_DEBUG
    qDebug() << "Returning -1";
#endif
    return -1;
}
int Oemp::close() {
    return serial_port.close() ? 0: -1;
}

int Oemp::checkCmdAckPkt(ushort wDevID, SB_OEM_PKT* pPkt) {
    if((pPkt->Head1 != STX1) ||  (pPkt->Head2 != STX2)) {
        return PKT_HDR_ERR;
    }

    if(pPkt->wDevId != wDevID) {
        return PKT_DEV_ID_ERR;
    }

    if(pPkt->wChkSum != calcChkSumOfCmdAckPkt(pPkt)) {
        return PKT_CHK_SUM_ERR;
    }

    return 0;
}

int Oemp::writeCmd(ushort wDevID, ushort wCmd, uint nParam) {
    SB_OEM_PKT pkt;
    int nSentBytes;

    pkt.Head1 = (uchar)STX1;
    pkt.Head2 = (uchar)STX2;
    pkt.wDevId = wDevID;
    pkt.wCmd = wCmd;
    pkt.nParam = nParam;
    pkt.wChkSum = calcChkSumOfCmdAckPkt(&pkt);

    nSentBytes = serial_port.write((uchar*)&pkt, SB_OEM_PKT_SIZE);

    if(nSentBytes != SB_OEM_PKT_SIZE) {
#ifdef OUTPUT_DEBUG
        qDebug() << "oemp_SendCmdOrAck, returning PKT_COMM_ERR";
#endif
        return PKT_COMM_ERR;
    }

    return 0;
}

int Oemp::readAck(ushort wDevID, ushort* wAck, int* pnParam) {
    SB_OEM_PKT pkt;
    int nReceivedBytes;

    if(!wAck || !pnParam) {
#ifdef OUTPUT_DEBUG
        qDebug() << "Parameter is null!" << (int)pwCmdOrAck << "," << (int)pnParam;
#endif
        return PKT_PARAM_ERR;
    }

#ifdef OUTPUT_DEBUG
    qDebug() << "trying to receive " << SB_OEM_PKT_SIZE << "bytes";
#endif

    nReceivedBytes = serial_port.read((uchar*)&pkt, SB_OEM_PKT_SIZE);

#ifdef OUTPUT_DEBUG
    qDebug() << "finally received:" << nReceivedBytes;
#endif

    if(nReceivedBytes != SB_OEM_PKT_SIZE) {
        return PKT_COMM_ERR;
    }

    if((pkt.Head1 != STX1) || (pkt.Head2 != STX2)) {
#ifdef OUTPUT_DEBUG
        qDebug() << "pkt.Head1=" << pkt.Head1 << ", STX1=" << STX1 << ", pkt.Head2=" << pkt.Head2 << ", STX2=" << STX2;
#endif
        return PKT_HDR_ERR;
    }

    if(pkt.wDevId != wDevID) {
        return PKT_DEV_ID_ERR;
    }

    if(pkt.wChkSum != calcChkSumOfCmdAckPkt(&pkt)) {
        return PKT_CHK_SUM_ERR;
    }

    *wAck = pkt.wCmd;
    *pnParam = pkt.nParam;

    return 0;
}

int Oemp::write(ushort wDevID, uchar* pBuf, int nSize) {
    ushort wChkSum = 0;
    uchar Buf[4], *pCommBuf;
    int nSentBytes;

    if(!pBuf) {
        return PKT_PARAM_ERR;
    }

    Buf[0] = (uchar)STX3;
    Buf[1] = (uchar)STX4;
    *((ushort*)(&Buf[SB_OEM_HEADER_SIZE])) = wDevID;

    wChkSum = calcChkSumOfDataPkt(Buf, SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE);
    wChkSum += calcChkSumOfDataPkt(pBuf, nSize);

    //nSentBytes = comm_send( Buf, SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE, gCommTimeOut  );
    //if( nSentBytes != SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE )
    //    return PKT_COMM_ERR;
    //
    //nSentBytes = comm_send( pBuf, nSize, gCommTimeOut );
    //if( nSentBytes != nSize )
    //    return PKT_COMM_ERR;
    //
    //nSentBytes = comm_send( (BYTE*)&wChkSum, SB_OEM_CHK_SUM_SIZE, gCommTimeOut );
    //if( nSentBytes != SB_OEM_CHK_SUM_SIZE )
    //    return PKT_COMM_ERR;

    ////////////// pc start ///////////////
    pCommBuf = new uchar[nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE+SB_OEM_CHK_SUM_SIZE];
    memcpy(pCommBuf, Buf, SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE);
    memcpy(pCommBuf+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE, pBuf, nSize);
    *(ushort*)(pCommBuf+nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE) = wChkSum;

    nSentBytes = serial_port.write(pCommBuf, nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE+SB_OEM_CHK_SUM_SIZE);
    if(nSentBytes != nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE+SB_OEM_CHK_SUM_SIZE) {
        if(pCommBuf) {
            delete pCommBuf;
        }
        return PKT_COMM_ERR;
    }

    if(pCommBuf) {
        delete pCommBuf;
    }
    ////////////// pc end ///////////////

    return 0;
}

int Oemp::read(ushort wDevID, uchar* pBuf, int nSize) {
    ushort wReceivedChkSum, wChkSum;
    uchar Buf[4], *pCommBuf;
    int nReceivedBytes;

    if(!pBuf) {
        return PKT_PARAM_ERR;
    }

#ifdef OUTPUT_DEBUG
    qDebug() << "oemp_ReceiveData, nSize=" << nSize;
#endif

    /*AVW modify*/
    pCommBuf = new uchar[nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE+SB_OEM_CHK_SUM_SIZE];
    nReceivedBytes = serial_port.read(pCommBuf, nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE+SB_OEM_CHK_SUM_SIZE);

#ifdef OUTPUT_DEBUG
    qDebug() << "oemp_ReceiveData, nReceivedBytes=" << nReceivedBytes;
#endif

    if(nReceivedBytes != nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE+SB_OEM_CHK_SUM_SIZE) {
        if(pCommBuf) {
            delete pCommBuf;
        }
        return PKT_COMM_ERR;
    }
    memcpy(Buf, pCommBuf, SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE);
    memcpy(pBuf, pCommBuf+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE, nSize);
    wReceivedChkSum = *(ushort*)(pCommBuf+nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE);
    if(pCommBuf) {
        delete pCommBuf;
    }
    ////////////// pc end ///////////////

    if((Buf[0] != STX3) || (Buf[1] != STX4)) {
        return PKT_HDR_ERR;
    }

    if(*((ushort*)(&Buf[SB_OEM_HEADER_SIZE])) != wDevID) {
        return PKT_DEV_ID_ERR;
    }

    wChkSum = calcChkSumOfDataPkt(Buf, SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE);
    wChkSum += calcChkSumOfDataPkt(pBuf, nSize);

    if(wChkSum != wReceivedChkSum) {
        return PKT_CHK_SUM_ERR;
    }
    /*AVW modify*/
    return 0;
}
