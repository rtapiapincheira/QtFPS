#-------------------------------------------------
#
# Project created by QtCreator 2014-12-29T19:11:13
#
#-------------------------------------------------

QT += core serialport

QT += gui
QT += widgets

TARGET   = QtFPSGui
TEMPLATE = app

RC_FILE += icons.rc

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    guihelper.cpp \

HEADERS += \
    mainwindow.h \
    guihelper.h

FORMS   += mainwindow.ui

DEFINES += MODE_WINDOW
DEFINES += ENABLED_QT5
DEFINES += QT_SERIAL_COMM
#DEFINES += RS232_SERIAL_COMM

#--------------------------------------------------------------------------------------------------#
#                       QtFPSConsole https://github.com/rtapiapincheira/QtFPS                      #
#--------------------------------------------------------------------------------------------------#
FPS_CONSOLE_PATH=../QtFPSConsole

INCLUDEPATH += \
    $${FPS_CONSOLE_PATH}/include \
    $${FPS_CONSOLE_PATH}/include/oem/impl

HEADERS += \
    $${FPS_CONSOLE_PATH}/include/argsparser.h \
    $${FPS_CONSOLE_PATH}/include/console_application.h \
    $${FPS_CONSOLE_PATH}/include/oem/impl/commbase_qt.h

SOURCES += \
    $${FPS_CONSOLE_PATH}/src/argsparser.cpp \
    $${FPS_CONSOLE_PATH}/src/console_application.cpp \
    $${FPS_CONSOLE_PATH}/src/oem/impl/commbase_qt.cpp

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
