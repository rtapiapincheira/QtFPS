///////////     Insert the code     ///////////
#include "OEM.h"

///////////////////////////////////////////////
#define COMM_DEF_TIMEOUT 15000
uint gCommTimeOut = COMM_DEF_TIMEOUT;

ushort oemp_CalcChkSumOfCmdAckPkt(SB_OEM_PKT* pPkt) {
    ushort wChkSum = 0;
    uchar* pBuf = (uchar*)pPkt;

    for (uint i=0; i<(sizeof(SB_OEM_PKT)-SB_OEM_CHK_SUM_SIZE); i++) {
		wChkSum += pBuf[i];
    }

	return wChkSum;
}

ushort oemp_CalcChkSumOfDataPkt(uchar* pDataPkt, int nSize) {
    ushort wChkSum = 0;
    uchar* pBuf = (uchar*)pDataPkt;
	
    for(int i=0; i<nSize; i++) {
        wChkSum += pBuf[i];
    }

	return wChkSum;
}

int oemp_CheckCmdAckPkt(ushort wDevID, SB_OEM_PKT* pPkt) {
    if((pPkt->Head1 != STX1) ||  (pPkt->Head2 != STX2)) {
		return PKT_HDR_ERR;
	}

    if(pPkt->wDevId != wDevID) {
		return PKT_DEV_ID_ERR;
    }

    if(pPkt->wChkSum != oemp_CalcChkSumOfCmdAckPkt(pPkt)) {
		return PKT_CHK_SUM_ERR;
    }

	return 0;
}

int oemp_SendCmdOrAck(ushort wDevID, ushort wCmdOrAck, uint nParam) {
	SB_OEM_PKT pkt;
	int nSentBytes;

    pkt.Head1 = (uchar)STX1;
    pkt.Head2 = (uchar)STX2;
	pkt.wDevId = wDevID;
	pkt.wCmd = wCmdOrAck;
	pkt.nParam = nParam;
    pkt.wChkSum = oemp_CalcChkSumOfCmdAckPkt(&pkt);

    nSentBytes = comm_send((uchar*)&pkt, SB_OEM_PKT_SIZE, gCommTimeOut);

    if(nSentBytes != SB_OEM_PKT_SIZE) {
        qDebug() << "oemp_SendCmdOrAck, returning PKT_COMM_ERR";
		return PKT_COMM_ERR;
    }

	return 0;
}

int oemp_ReceiveCmdOrAck(ushort wDevID, ushort* pwCmdOrAck, int* pnParam) {
	SB_OEM_PKT pkt;
	int nReceivedBytes;
		
    if(!pwCmdOrAck || !pnParam) {
        qDebug() << "Parameter is null!" << (int)pwCmdOrAck << "," << (int)pnParam;
		return PKT_PARAM_ERR;
	}

    //qDebug() << "trying to receive " << SB_OEM_PKT_SIZE << "bytes, with a timeout:" << gCommTimeOut;

    nReceivedBytes = comm_recv((uchar*)&pkt, SB_OEM_PKT_SIZE, gCommTimeOut);

    //qDebug() << "finally received:" << nReceivedBytes;

    if(nReceivedBytes != SB_OEM_PKT_SIZE) {
		return PKT_COMM_ERR;
    }

    if((pkt.Head1 != STX1) || (pkt.Head2 != STX2)) {
        qDebug() << "pkt.Head1=" << pkt.Head1 << ", STX1=" << STX1 << ", pkt.Head2=" << pkt.Head2 << ", STX2=" << STX2;
		return PKT_HDR_ERR;
	}

    if(pkt.wDevId != wDevID) {
		return PKT_DEV_ID_ERR;
    }

    if(pkt.wChkSum != oemp_CalcChkSumOfCmdAckPkt(&pkt)) {
		return PKT_CHK_SUM_ERR;
    }

	*pwCmdOrAck = pkt.wCmd;
	*pnParam = pkt.nParam;

	return 0;
}

int oemp_SendData(ushort wDevID, uchar* pBuf, int nSize) {
    ushort wChkSum = 0;
    uchar Buf[4], *pCommBuf;
	int nSentBytes;
	
    if(!pBuf) {
		return PKT_PARAM_ERR;
    }

    Buf[0] = (uchar)STX3;
    Buf[1] = (uchar)STX4;
    *((ushort*)(&Buf[SB_OEM_HEADER_SIZE])) = wDevID;
	
	wChkSum = oemp_CalcChkSumOfDataPkt( Buf, SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE  );
	wChkSum += oemp_CalcChkSumOfDataPkt( pBuf, nSize );
	
	//nSentBytes = comm_send( Buf, SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE, gCommTimeOut  );
	//if( nSentBytes != SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE )
	//	return PKT_COMM_ERR;
	//
	//nSentBytes = comm_send( pBuf, nSize, gCommTimeOut );
	//if( nSentBytes != nSize )
	//	return PKT_COMM_ERR;
	//
	//nSentBytes = comm_send( (BYTE*)&wChkSum, SB_OEM_CHK_SUM_SIZE, gCommTimeOut );
	//if( nSentBytes != SB_OEM_CHK_SUM_SIZE )
	//	return PKT_COMM_ERR;

	////////////// pc start ///////////////
    pCommBuf = new uchar[nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE+SB_OEM_CHK_SUM_SIZE];
	memcpy(pCommBuf, Buf, SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE);
	memcpy(pCommBuf+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE, pBuf, nSize);
    *(ushort*)(pCommBuf+nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE) = wChkSum;

    nSentBytes = comm_send(pCommBuf, nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE+SB_OEM_CHK_SUM_SIZE, gCommTimeOut);
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

int oemp_ReceiveData(ushort wDevID, uchar* pBuf, int nSize) {
    ushort wReceivedChkSum, wChkSum;
    uchar Buf[4], *pCommBuf;
	int nReceivedBytes;
	
    if(!pBuf) {
		return PKT_PARAM_ERR;
    }

    qDebug() << "oemp_ReceiveData, nSize=" << nSize;

	/*AVW modify*/
    pCommBuf = new uchar[nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE+SB_OEM_CHK_SUM_SIZE];
    nReceivedBytes = comm_recv(pCommBuf, nSize+SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE+SB_OEM_CHK_SUM_SIZE, gCommTimeOut);

    qDebug() << "oemp_ReceiveData, nReceivedBytes=" << nReceivedBytes;

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
	
    wChkSum = oemp_CalcChkSumOfDataPkt(Buf, SB_OEM_HEADER_SIZE+SB_OEM_DEV_ID_SIZE);
    wChkSum += oemp_CalcChkSumOfDataPkt(pBuf, nSize);
	
    if(wChkSum != wReceivedChkSum) {
		return PKT_CHK_SUM_ERR;
    }
	/*AVW modify*/
	return 0;
}
