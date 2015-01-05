#-------------------------------------------------
#
# Project created by QtCreator 2014-12-29T19:11:13
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = QtFPS
TEMPLATE = app

RC_FILE += icons.rc

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    helper.cpp \
    handler.cpp \
    \
    oem/commbase.cpp \
    oem/oem.cpp \
    oem/sbprotocoloem.cpp

HEADERS += \
    mainwindow.h \
    helper.h \
    handler.h \
    \
    oem/commbase.h \
    oem/oem.h \
    oem/sbprotocoloem.h

FORMS   += mainwindow.ui
