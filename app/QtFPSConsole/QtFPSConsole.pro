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
    ../QtFPSGui \
    ../QtFPSGui/oem \
    ../QtFPSGui/oem/impl

SOURCES += \
    main.cpp \
    argsparser.cpp \
    console_application.cpp \
    \
    ../QtFPSGui/oem/commbase.cpp \
    ../QtFPSGui/oem/impl/commbase_qt.cpp \
    ../QtFPSGui/oem/impl/commbase_rs232.cpp \
    ../QtFPSGui/oem/oem.cpp \
    ../QtFPSGui/oem/oemp.cpp \
    \
    ../QtFPSGui/controller.cpp \
    ../QtFPSGui/helper.cpp

HEADERS += \
    argsparser.h \
    console_application.h \
    \
    ../QtFPSGui/oem/commbase.h \
    ../QtFPSGui/oem/impl/commbase_qt.h \
    ../QtFPSGui/oem/impl/commbase_rs232.h \
    ../QtFPSGui/oem/oem.h \
    ../QtFPSGui/oem/oemp.h \
    \
    ../QtFPSGui/config.h \
    ../QtFPSGui/controller.h \
    ../QtFPSGui/helper.h

DEFINES += ENABLED_QT5
DEFINES += QT_SERIAL_COMM
#DEFINES += RS232_SERIAL_COMM

# Note: if you want to compile this application without using Qt5 specific features (like
# QtSerialPort), see the ../QtFPSConsoleRpi related project which is meant to be compiled using Qt
# 4.8 under non GUI environment.

#--------------------------------------------------------------------------------------------------#
#                                   http://www.teuniz.net/RS-232/                                  #
#--------------------------------------------------------------------------------------------------#

INCLUDEPATH += ../QtFPSGui/RS232
HEADERS     += ../QtFPSGui/RS232/rs232.h
SOURCES     += ../QtFPSGui/RS232/rs232.c
