#-------------------------------------------------
#
# Project created by QtCreator 2015-01-05T20:56:04
#
#-------------------------------------------------

QT       += core serialport
#QT       -= gui

TARGET   = QtFPSConsole
CONFIG  += console
CONFIG  -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../QtFPS \
    ../QtFPS/oem

SOURCES += \
    main.cpp \
    \
    ../QtFPS/oem/commbase.cpp \
    ../QtFPS/oem/oem.cpp \
    ../QtFPS/oem/sbprotocoloem.cpp \
    \
    ../QtFPS/controller.cpp \
    ../QtFPS/helper.cpp

HEADERS += \
    ../QtFPS/oem/commbase.h \
    ../QtFPS/oem/oem.h \
    ../QtFPS/oem/sbprotocoloem.h \
    \
    ../QtFPS/config.h \
    ../QtFPS/controller.h \
    ../QtFPS/helper.h
