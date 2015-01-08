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
    oem \
    oem/impl

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    helper.cpp \
    controller.cpp \
    \
    oem/commbase.cpp \
    oem/impl/commbase_qt.cpp \
    oem/impl/commbase_rs232.cpp \
    oem/oem.cpp \
    oem/oemp.cpp

HEADERS += \
    mainwindow.h \
    helper.h \
    controller.h \
    config.h\
    \
    oem/commbase.h \
    oem/impl/commbase_qt.h \
    oem/impl/commbase_rs232.h \
    oem/oem.h \
    oem/oemp.h

FORMS   += mainwindow.ui

DEFINES += MODE_WINDOW
DEFINES += ENABLED_QT5
DEFINES += QT_SERIAL_COMM
#DEFINES += RS232_SERIAL_COMM

#--------------------------------------------------------------------------------------------------#
#                                   http://www.teuniz.net/RS-232/                                  #
#--------------------------------------------------------------------------------------------------#

INCLUDEPATH += RS232
HEADERS     += RS232/rs232.h
SOURCES     += RS232/rs232.c
