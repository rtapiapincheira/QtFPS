#-------------------------------------------------
#
# Project created by QtCreator 2014-12-29T19:11:13
#
#-------------------------------------------------

QT += core serialport

QT += gui
QT += widgets

#CONFIG += console

TARGET   = QtFPS
TEMPLATE = app

RC_FILE += icons.rc

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
    oem/commbase.h \
    oem/oem.h \
    oem/sbprotocoloem.h

FORMS   += mainwindow.ui
