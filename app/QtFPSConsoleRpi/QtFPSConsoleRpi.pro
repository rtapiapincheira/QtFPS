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
    ../QtFPSGui \
    ../QtFPSGui/oem \
    ../QtFPSGui/oem/impl

SOURCES += \
    ../QtFPSConsole/main.cpp \
    ../QtFPSConsole/argsparser.cpp \
    ../QtFPSConsole/console_application.cpp \
    \
    ../QtFPSGui/oem/commbase.cpp \
    ../QtFPSGui/oem/impl/commbase_qt.cpp \
    ../QtFPSGui/oem/impl/commbase_rs232.cpp \
    ../QtFPSGui/oem/oem.cpp \
    ../QtFPSGui/oem/oemp.cpp \
    \
    ../QtFPS/controller.cpp \
    ../QtFPS/helper.cpp

HEADERS += \
    ../QtFPSConsole/argsparser.h \
    ../QtFPSConsole/console_application.h \
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

INCLUDEPATH += ../QtFPSGui/RS232
HEADERS     += ../QtFPSGui/RS232/rs232.h
SOURCES     += ../QtFPSGui/RS232/rs232.c
