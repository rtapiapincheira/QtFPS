#-------------------------------------------------
#
# Project created by QtCreator 2015-01-05T20:56:04
#
#-------------------------------------------------

QT       += core serialport

TARGET   = QtFPSConsole
CONFIG  += console
CONFIG  -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../QtFPS \
    ../QtFPS/oem \
    ../QtFPS/oem/impl

SOURCES += \
    main.cpp \
    argsparser.cpp \
    console_application.cpp \
    \
    ../QtFPS/oem/commbase.cpp \
    ../QtFPS/oem/impl/commbase_qt.cpp \
    ../QtFPS/oem/impl/commbase_rs232.cpp \
    ../QtFPS/oem/oem.cpp \
    ../QtFPS/oem/oemp.cpp \
    \
    ../QtFPS/controller.cpp \
    ../QtFPS/helper.cpp

HEADERS += \
    argsparser.h \
    console_application.h \
    \
    ../QtFPS/oem/commbase.h \
    ../QtFPS/oem/impl/commbase_qt.h \
    ../QtFPS/oem/impl/commbase_rs232.h \
    ../QtFPS/oem/oem.h \
    ../QtFPS/oem/oemp.h \
    \
    ../QtFPS/config.h \
    ../QtFPS/controller.h \
    ../QtFPS/helper.h

DEFINES += ENABLED_QT5
DEFINES += QT_SERIAL_COMM
#DEFINES += RS232_SERIAL_COMM

# Note: if you want to compile this application without using Qt5 specific features (like
# QtSerialPort), see the ../QtFPSConsoleRpi related project which is meant to be compiled using Qt
# 4.8 under non GUI environment.

#--------------------------------------------------------------------------------------------------#
#                                   http://www.teuniz.net/RS-232/                                  #
#--------------------------------------------------------------------------------------------------#

INCLUDEPATH += ../QtFPS/RS232
HEADERS     += ../QtFPS/RS232/rs232.h
SOURCES     += ../QtFPS/RS232/rs232.c
