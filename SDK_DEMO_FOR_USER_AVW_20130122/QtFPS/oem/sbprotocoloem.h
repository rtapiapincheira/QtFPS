#ifndef __SB_PROTOCOL_OEM_H__
#define __SB_PROTOCOL_OEM_H__

#include <QtGlobal>

// Header Of Cmd and Ack Packets
#define STX1				0x55	//Header1 
#define STX2				0xAA	//Header2

// Header Of Data Packet
#define STX3				0x5A	//Header1 
#define STX4				0xA5	//Header2

// Structure Of Cmd and Ack Packets 
typedef struct {		
    uchar 	Head1;
    uchar 	Head2;
    ushort	wDevId;
	int		nParam;
    ushort	wCmd;// or nAck
    ushort 	wChkSum;
} SB_OEM_PKT;			

#define SB_OEM_PKT_SIZE			12
#define SB_OEM_HEADER_SIZE		2
#define SB_OEM_DEV_ID_SIZE		2
#define SB_OEM_CHK_SUM_SIZE		2

#define PKT_ERR_START	-500
#define PKT_COMM_ERR	PKT_ERR_START+1
#define PKT_HDR_ERR		PKT_ERR_START+2
#define PKT_DEV_ID_ERR	PKT_ERR_START+3
#define PKT_CHK_SUM_ERR	PKT_ERR_START+4
#define PKT_PARAM_ERR	PKT_ERR_START+5

int oemp_CheckCmdAckPkt(ushort wDevID, SB_OEM_PKT* pPkt);
int oemp_SendCmdOrAck(ushort wDevID, ushort wCmdOrAck, int nParam);
int oemp_ReceiveCmdOrAck(ushort wDevID, ushort* pwCmdOrAck, int* pnParam);
int oemp_SendData(ushort wDevID, uchar* pBuf, int nSize);
int oemp_ReceiveData(ushort wDevID, uchar* pBuf, int nSize);

#endif