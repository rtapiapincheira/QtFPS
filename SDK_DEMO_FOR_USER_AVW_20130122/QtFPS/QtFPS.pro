#-------------------------------------------------
#
# Project created by QtCreator 2014-12-29T19:11:13
#
#-------------------------------------------------

QT += core serialport

QT += gui
QT += widgets

TARGET   = QtFPS
TEMPLATE = app

RC_FILE += icons.rc

INCLUDEPATH += \
    oem

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    helper.cpp \
    controller.cpp \
    \
    oem/commbase.cpp \
    oem/oem.cpp \
    oem/sbprotocoloem.cpp

HEADERS += \
    mainwindow.h \
    helper.h \
    controller.h \
    config.h\
    \
    commbase.h \
    oem.h \
    sbprotocoloem.h

FORMS   += mainwindow.ui

DEFINES += MODE_WINDOW
