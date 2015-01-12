#include "config.h"
#include "helper.h"
#include "controller.h"

#include "commbase_rs232.h"

void handler(void *p) {
    // Do nothing!
}

int main(int argc, char **argv) {

    Helper h;
    h.consoleConfig.port = "COM2";
    h.consoleConfig.baudrate = 115200;
    h.consoleConfig.filename = "output.png";

    Rs232CommSerial serialPort;

    Controller c(&serialPort);
    c.setup(&h, &handler);

    c.__open();
    c.__getRawImage();
    c.__saveImageToFile();
    c.__close();

    return 0;
}
