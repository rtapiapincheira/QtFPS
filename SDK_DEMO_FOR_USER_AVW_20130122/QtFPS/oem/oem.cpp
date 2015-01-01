#include "OEM.h"

//////////////////////////////////////////////////////////////////////////
uchar	gbyImg8bit[IMG8BIT_SIZE];
uchar	gbyImgRaw[320*240];
uchar	gbyTemplate[FP_TEMPLATE_SIZE];

ushort gwDevID = 1;
ushort gwLastAck = 0;
int gwLastAckParam = 0;

devinfo gDevInfo;

//////////////////////////////////////////////////////////////////////////
int oem_CommandRun(ushort wCmd, int nCmdParam) {
    if(oemp_SendCmdOrAck(gwDevID, wCmd, nCmdParam) < 0) {
        qDebug() << "returning OEM_COMM_ERR (if #1)";
		return OEM_COMM_ERR;
    }
    if(oemp_ReceiveCmdOrAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
        qDebug() << "returning OEM_COMM_ERR (if #2)";
		return OEM_COMM_ERR;
    }
    //qDebug() << "returning 0 (OK)";
	return 0;
}

int oem_open() {
    if(oem_CommandRun(CMD_OPEN, 1) < 0) {
		return OEM_COMM_ERR;
    }

    if(oemp_ReceiveData(gwDevID, (uchar*)&gDevInfo, sizeof(devinfo)) < 0) {
		return OEM_COMM_ERR;
    }

	return 0;
}

int oem_close() {
    return oem_CommandRun(CMD_CLOSE, 0);
}

extern uint gCommTimeOut;

int oem_usb_internal_check() {
	
    uint prevCommTimeOut = gCommTimeOut;
	gCommTimeOut = 1000;
	
    int ret = oem_CommandRun(CMD_USB_INTERNAL_CHECK, gwDevID);
	
	gCommTimeOut = prevCommTimeOut;
	return ret;
}

int oem_change_baudrate(int nBaudrate) {
    return oem_CommandRun(CMD_CHANGE_BAUDRATE, nBaudrate);
}

int oem_cmos_led(bool bOn) {
    return oem_CommandRun(CMD_CMOS_LED, bOn ? 1 : 0);
}

int oem_enroll_count() {
    return oem_CommandRun(CMD_ENROLL_COUNT, 0);
}

int oem_check_enrolled(int nPos) {
    return oem_CommandRun(CMD_CHECK_ENROLLED, nPos);
}

int oem_enroll_start(int nPos) {
    return oem_CommandRun(CMD_ENROLL_START, nPos);
}

/*AVW*/
int oem_enroll_nth(int nPos, int nTurn) {
    if (oem_CommandRun(CMD_ENROLL_START+nTurn, 0) < 0) {
        qDebug() << "oem_enroll_nth: returning OEM_COMM_ERR";
		return OEM_COMM_ERR;
    }

    if (nPos == -1 && nTurn == 3) {
        if(gwLastAck == ACK_OK) {
            if(oemp_ReceiveData(gwDevID, &gbyTemplate[0], FP_TEMPLATE_SIZE) < 0) {
                qDebug() << "oem_enroll_nth: returning OEM_COMM_ERR";
				return OEM_COMM_ERR;
            }
		}
	}

	return 0;
}

int oem_is_press_finger() {
    return oem_CommandRun(CMD_IS_PRESS_FINGER, 0);
}

int oem_delete(int nPos) {
    return oem_CommandRun(CMD_DELETE, nPos);
}

int oem_delete_all() {
    return oem_CommandRun(CMD_DELETE_ALL, 0);
}

int oem_verify(int nPos) {
    return oem_CommandRun(CMD_VERIFY, nPos);
}

int oem_identify() {
    return oem_CommandRun(CMD_IDENTIFY, 0);
}

int oem_verify_template(int nPos) {
    if (oem_CommandRun(CMD_VERIFY_TEMPLATE, nPos) < 0) {
		return OEM_COMM_ERR;
    }
	
    if (gwLastAck == ACK_OK) {
        if(oemp_SendData(gwDevID, &gbyTemplate[0], FP_TEMPLATE_SIZE) < 0) {
			return OEM_COMM_ERR;
        }
        if(oemp_ReceiveCmdOrAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
			return OEM_COMM_ERR;
        }
	}
	return 0;
}

int oem_identify_template() {
    if(oem_CommandRun( CMD_IDENTIFY_TEMPLATE, 0) < 0) {
		return OEM_COMM_ERR;
    }
	
    if(gwLastAck == ACK_OK) {
        if(oemp_SendData(gwDevID, &gbyTemplate[0], FP_TEMPLATE_SIZE) < 0) {
			return OEM_COMM_ERR;
        }
        if(oemp_ReceiveCmdOrAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
			return OEM_COMM_ERR;
        }
	}
	
	return 0;
}

int oem_capture(bool bBest) {
    return oem_CommandRun(CMD_CAPTURE, bBest);
}

/*AVW*/
static uchar gbyImg256_2[202*258];
static uchar gbyImg256_tmp[258*202];

int oem_get_image() {
    if(oem_CommandRun( CMD_GET_IMAGE, 0) < 0){
		return OEM_COMM_ERR;
    }

    if(oemp_ReceiveData( gwDevID, gbyImg256_tmp, sizeof(gbyImg256_tmp)) < 0) {
		return OEM_COMM_ERR;
    }

	// image rotate
	int i, j;

    for(i=0; i<202; i++) {
        for(j=0; j<258; j++) {
			gbyImg256_2[i*258+j] = gbyImg256_tmp[j*202+i];
		}
	}
	
	memset(gbyImg8bit, 161, sizeof(gbyImg8bit));
	
    for(i=0; i<202; i++) {
		memcpy(&gbyImg8bit[256*(27 + i) + 0], &gbyImg256_2[i * 258 + 1], 256);
	}
	
	return 0;
}

/*AVW*/
static uchar gbyImgRaw2[240*320/4];

int oem_get_rawimage() {
    if(oem_CommandRun(CMD_GET_RAWIMAGE, 0) < 0) {
		return OEM_COMM_ERR;
    }
	
    if(oemp_ReceiveData(gwDevID, gbyImgRaw2, sizeof(gbyImgRaw2)) < 0) {
		return OEM_COMM_ERR;
    }

	/*AVW*/
	memset(gbyImgRaw, 66, sizeof gbyImgRaw);
	int i, j;
    for(i=0; i<120; i++) {
        for(j=0; j<160; j++) {
			gbyImgRaw[320*(2*i+0)+(2*j+0)] = gbyImgRaw2[i*160+j];
			gbyImgRaw[320*(2*i+0)+(2*j+1)] = gbyImgRaw2[i*160+j];
			gbyImgRaw[320*(2*i+1)+(2*j+0)] = gbyImgRaw2[i*160+j];
			gbyImgRaw[320*(2*i+1)+(2*j+1)] = gbyImgRaw2[i*160+j];
		}		
	}

	return 0;
}

int oem_get_template(int nPos)
{
    if(oem_CommandRun(CMD_GET_TEMPLATE, nPos) < 0) {
		return OEM_COMM_ERR;
    }

    if(gwLastAck == ACK_OK) {
        if(oemp_ReceiveData(gwDevID, &gbyTemplate[0], FP_TEMPLATE_SIZE) < 0) {
			return OEM_COMM_ERR;
        }
	}
	
	return 0;
}

int oem_add_template(int nPos)
{
    if(oem_CommandRun( CMD_ADD_TEMPLATE, nPos) < 0) {
		return OEM_COMM_ERR;
    }
	
    if(gwLastAck == ACK_OK) {
        if(oemp_SendData(gwDevID, &gbyTemplate[0], FP_TEMPLATE_SIZE) < 0) {
			return OEM_COMM_ERR;
        }
        if(oemp_ReceiveCmdOrAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
			return OEM_COMM_ERR;
        }
	}
	
	return 0;
}

int oem_get_database_end() {
    return oem_CommandRun(CMD_GET_DATABASE_END, 0);
}

int oem_get_database_start(){
    return oem_CommandRun(CMD_GET_DATABASE_START, 0);
}

int oem_fw_upgrade(uchar* pBuf, int nLen)
{
    if(oem_CommandRun(CMD_FW_UPDATE, nLen) < 0) {
		return OEM_COMM_ERR;
    }
	int nSegSize = 0;
	int cbWrote = 0;
    if(gwLastAck == ACK_OK) {
		nSegSize = gwLastAckParam;	// flash segment size that's returned with ack.
        if(nSegSize <= 0) {
			return OEM_COMM_ERR;
		}

		/* send firmware */	
        while (cbWrote < nLen) {
            if (cbWrote + nSegSize > nLen) {
                nSegSize = nLen - cbWrote;
            }
            if (oemp_SendData(gwDevID, &pBuf[cbWrote], nSegSize) < 0) {
				return OEM_COMM_ERR;
            }
            if(oemp_ReceiveCmdOrAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
                return OEM_COMM_ERR;
			}
			cbWrote += nSegSize;
		}
	}
	return 0;
}

int oem_iso_upgrade(uchar* pBuf, int nLen)
{
    if(oem_CommandRun(CMD_ISO_UPDATE, nLen) < 0) {
		return OEM_COMM_ERR;
    }
	int nSegSize = 0;
	int cbWrote = 0;
    if(gwLastAck == ACK_OK) {
		nSegSize = gwLastAckParam;	// flash segment size that's returned with ack.
        if(nSegSize <= 0){
			return OEM_COMM_ERR;
		}
		
		/* send firmware */	
        while (cbWrote < nLen) {
            if (cbWrote + nSegSize > nLen) {
                nSegSize = nLen - cbWrote;
            }
			
            if (oemp_SendData(gwDevID, &pBuf[cbWrote], nSegSize) < 0) {
				return OEM_COMM_ERR;
            }
            if (oemp_ReceiveCmdOrAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
				return OEM_COMM_ERR;
			}
			cbWrote += nSegSize;
		}
	}
	return 0;
}

QString my_oem_error(int nNackInfo, int nPos)
{
    if(nNackInfo > NACK_NONE) {
        switch(nNackInfo) {
        case NACK_TIMEOUT:
            return "Time Out!";
        case NACK_INVALID_BAUDRATE:
            return "Invalid baudrate";
        case NACK_INVALID_POS:
            return "Invalid ID";
        case NACK_IS_NOT_USED:
            return QString("ID = %1: is not used!").arg(nPos);
        case NACK_IS_ALREADY_USED:
            return QString("ID = %1: is already used!").arg(nPos);
        case NACK_COMM_ERR:
            return "Communication error!";
        case NACK_VERIFY_FAILED:
            return QString("ID = %1: NG!").arg(nPos);
        case NACK_IDENTIFY_FAILED:
            return "NG!";
        case NACK_DB_IS_FULL:
            return "Database is full";
        case NACK_DB_IS_EMPTY:
            return "Database is empty";
        case NACK_TURN_ERR:
            return "The order of enrollment is incorrect!";
        case NACK_BAD_FINGER:
            return "Bad finger!";
        case NACK_ENROLL_FAILED:
            return "The enrollment is failed!";
        case NACK_IS_NOT_SUPPORTED:
            return "The command is not supported";
        case NACK_DEV_ERR:
            return "The device error!";
        case NACK_CAPTURE_CANCELED:
            return "Canceled!";
        case NACK_INVALID_PARAM:
            return "Invalid Firmware Image!";
        case NACK_FINGER_IS_NOT_PRESSED:
            return "Finger is not pressed!";
        default:
            return "Unknown Error";
        }
    } else if (nNackInfo < FP_MAX_USERS) {
        return QString("ID = %1: is already enrolled!").arg(nNackInfo);
    }
    return "I don't know!!!";
}

int my_oem_capturing(bool best, QString &err) {
    qint64 st = QDateTime::currentMSecsSinceEpoch();
    while(QDateTime::currentMSecsSinceEpoch() - st < 5000) {
        if(oem_capture(best ? 1 : 0) < 0) {
            err = "Communication error!";
            return -1;
        } else if(gwLastAck == ACK_OK) {
            return 0;
        }
    }
    err = "Timeout!";
    return -2;
}
