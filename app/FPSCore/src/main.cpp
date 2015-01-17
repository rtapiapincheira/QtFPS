#include "config.h"
#include "helper.h"
#include "controller.h"

#include "commbase_rs232.h"

#define FF fflush(stdout)

void handler(void *p) {
    // Do nothing!
}

int main(int argc, char **argv) {

    Helper h;
    h.consoleConfig.port = "/dev/ttyAMA0";
    h.consoleConfig.baudrate = 115200;
    h.consoleConfig.filename = "output_x.png";

    Rs232CommSerial serialPort;

    printf("Setting controller up... "); FF;
    Controller c(&serialPort);
    c.setup(&h, &handler);
    printf("set up.\n"); FF;

    printf("Opening... "); FF;
    c.__open();
    printf("Open.\n"); FF;

    for(char d = 'A'; d <= 'J'; d++) {
        h.consoleConfig.filename[7] = d;

        printf("Getting raw image (%s) ...", h.consoleConfig.filename.c_str()); FF;
        c.__getRawImage();
        printf("Gotcha.\n"); FF;

        printf("Saving image to file... "); FF;
        c.__saveImageToFile();
        printf("Saved.\n"); FF;
    }

    printf("Closing... "); FF;
    c.__close();
    printf("Closed.\n"); FF;

    return 0;
}
