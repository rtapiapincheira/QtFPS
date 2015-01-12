#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T11:59:39
#
#-------------------------------------------------

QT       += core

TARGET   = QtFPSConsoleRpi
CONFIG  += console
CONFIG  -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../QtFPS \
    ../QtFPS/oem \
    ../QtFPS/oem/impl

SOURCES += \
    ../QtFPSConsole/main.cpp \
    ../QtFPSConsole/argsparser.cpp \
    ../QtFPSConsole/console_application.cpp \
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
    ../QtFPSConsole/argsparser.h \
    ../QtFPSConsole/console_application.h \
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

DEFINES += RS232_SERIAL_COMM

#--------------------------------------------------------------------------------------------------#
#                 Portable Serial communication library http://wjwwood.io/serial/                  #
#--------------------------------------------------------------------------------------------------#

#INCLUDEPATH += wjwwood/serial/include
#HEADERS     += \
#    wjwwood/serial/include/serial/serial.h \
#    wjwwood/serial/include/serial/v8stdint.h \
#    wjwwood/serial/include/serial/impl/unix.h \
#    wjwwood/serial/include/serial/impl/win.h
#SOURCES     += \
#    wjwwood/serial/src/serial.cc \
#    wjwwood/serial/src/impl/unix.cc \
#    wjwwood/serial/src/impl/win.cc \
#    wjwwood/serial/src/impl/list_ports/list_ports_linux.cc \
#    wjwwood/serial/src/impl/list_ports/list_ports_osx.cc \
#    wjwwood/serial/src/impl/list_ports/list_ports_win.cc
# Needs liking with Windows OS static libraries


#--------------------------------------------------------------------------------------------------#
#                                   http://www.teuniz.net/RS-232/                                  #
#--------------------------------------------------------------------------------------------------#

INCLUDEPATH += ../QtFPS/RS232
HEADERS     += ../QtFPS/RS232/rs232.h
SOURCES     += ../QtFPS/RS232/rs232.c
