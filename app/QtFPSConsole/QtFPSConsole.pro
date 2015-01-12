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
    include \
    include\oem\impl

HEADERS += \
    include/argsparser.h \
    include/console_application.h \
    \
    include/oem/impl/commbase_qt.h

SOURCES += \
    src/main.cpp \
    src/argsparser.cpp \
    src/console_application.cpp \
    \
    src/oem/impl/commbase_qt.cpp

DEFINES += ENABLED_QT5
DEFINES += QT_SERIAL_COMM
#DEFINES += RS232_SERIAL_COMM

# Note: if you want to compile this application without using Qt5 specific features (like
# QtSerialPort), see the ../QtFPSConsoleRpi related project which is meant to be compiled using Qt
# 4.8 under non GUI environment.

#--------------------------------------------------------------------------------------------------#
#                         FPSCore https://github.com/rtapiapincheira/QtFPS                         #
#--------------------------------------------------------------------------------------------------#
FPS_PATH=../FPSCore

INCLUDEPATH += \
    $${FPS_PATH}/include \
    $${FPS_PATH}/include/oem \
    $${FPS_PATH}/include/oem/impl

HEADERS += \
    $${FPS_PATH}/include/config.h \
    $${FPS_PATH}/include/controller.h \
    $${FPS_PATH}/include/helper.h \
    $${FPS_PATH}/include/utils.h \
    \
    $${FPS_PATH}/include/oem/commbase.h \
    $${FPS_PATH}/include/oem/oem.h \
    $${FPS_PATH}/include/oem/oemp.h
    \
    $${FPS_PATH}/include/oem/impl/commbase_rs232.h

SOURCES += \
    $${FPS_PATH}/src/controller.cpp \
    $${FPS_PATH}/src/helper.cpp \
    $${FPS_PATH}/src/utils.cpp \
    \
    $${FPS_PATH}/src/oem/commbase.cpp \
    $${FPS_PATH}/src/oem/oem.cpp \
    $${FPS_PATH}/src/oem/oemp.cpp \
    \
    $${FPS_PATH}/src/oem/impl/commbase_rs232.cpp

#--------------------------------------------------------------------------------------------------#
#                            RS-232 http://www.teuniz.net/RS-232/                                  #
#--------------------------------------------------------------------------------------------------#

INCLUDEPATH += $${FPS_PATH}/RS232
HEADERS     += $${FPS_PATH}/RS232/rs232.h
SOURCES     += $${FPS_PATH}/RS232/rs232.c

#--------------------------------------------------------------------------------------------------#
#                             LodePNG  http://lodev.org/lodepng/                                   #
#--------------------------------------------------------------------------------------------------#

INCLUDEPATH += $${FPS_PATH}/LodePNG
HEADERS     += $${FPS_PATH}/LodePNG/lodepng.h
SOURCES     += $${FPS_PATH}/LodePNG/lodepng.cpp
