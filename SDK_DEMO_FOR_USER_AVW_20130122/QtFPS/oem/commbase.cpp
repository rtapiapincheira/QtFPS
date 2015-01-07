#include "oem.h"

CCommSerial::CCommSerial() {
}

CCommSerial::~CCommSerial() {

}

void CCommSerial::setCallback(void(*listener)(void*p), void *parameter) {
    m_listener = listener;
    m_parameter = parameter;
}

void CCommSerial::setTimeout(uint dwTimeOut) {
    timeOut = dwTimeOut;
}
