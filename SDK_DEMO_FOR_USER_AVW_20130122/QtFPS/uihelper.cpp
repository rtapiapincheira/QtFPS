#include "uihelper.h"

UiHelper::UiHelper() {
}

UiHelper::~UiHelper() {

}

void UiHelper::setup(Ui::MainWindowDialog *mw) {
    canvas = mw->canvas;

    saveImageToFile = mw->save_image_to_file;

    result = mw->result;

    serialPortNumber = mw->serial_port_number;
    baudrate = mw->baudrate;

    open = mw->open;
    close = mw->close;

    id = mw->id;

    enroll = mw->enroll;
    getUserCount = mw->get_user_count;
    verify = mw->verify_1_1;
    deleteId = mw->delete_id;
    identify = mw->identify_1_n;
    deleteAll = mw->delete_all;
    verifyTemplate = mw->verify_template;
    getTemplate = mw->get_template;
    identifyTemplate = mw->identify_template;
    setTemplate = mw->set_template;
    isPressedFinger = mw->is_press_finger;
    getDatabase = mw->get_database;

    getImage = mw->get_image;
    setDatabase = mw->set_database;
    getRawImage = mw->get_raw_image;
    cancel = mw->cancel;

    getLiveImage = mw->get_live_image;
}
