#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T21:25:45
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = FPSCore
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp

DEFINES += RS232_SERIAL_COMM

#--------------------------------------------------------------------------------------------------#
#                         FPSCore https://github.com/rtapiapincheira/QtFPS                         #
#--------------------------------------------------------------------------------------------------#

INCLUDEPATH += \
    include \
    include/oem \
    include/oem/impl

SOURCES += \
    src/controller.cpp \
    src/helper.cpp \
    src/utils.cpp \
    \
    src/oem/commbase.cpp \
    src/oem/oem.cpp \
    src/oem/oemp.cpp \
    \
    src/oem/impl/commbase_rs232.cpp

HEADERS += \
    include/config.h \
    include/controller.h \
    include/helper.h \
    include/utils.h \
    \
    include/oem/commbase.h \
    include/oem/oem.h \
    include/oem/oemp.h \
    \
    include/oem/impl/commbase_rs232.h \

#--------------------------------------------------------------------------------------------------#
#                                   http://www.teuniz.net/RS-232/                                  #
#--------------------------------------------------------------------------------------------------#

INCLUDEPATH += RS232
HEADERS     += RS232/rs232.h
SOURCES     += RS232/rs232.c

#--------------------------------------------------------------------------------------------------#
#                                     http://lodev.org/lodepng/                                    #
#--------------------------------------------------------------------------------------------------#

INCLUDEPATH += LodePNG
HEADERS     += LodePNG/lodepng.h
SOURCES     += LodePNG/lodepng.cpp

