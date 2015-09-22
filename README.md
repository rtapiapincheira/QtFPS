QtFPS
=====

Qt application for Finger Print Sensor GT-511C3

This project was developed using the latest QtCreator (3.1.2 at the time of
writing of this doc) and Qt SDK version 5.3.1.

It uses the Qt5 serial library (if available) and it can fall back to a simple RS232 serial
communication http://www.teuniz.net/RS-232/.

MacOSX compiling instructions
=====
1) Install QtCreator for Mac
http://www.qt.io/download-open-source/
make sure is 5.x version and does include all Qt SDK libraries
2) Install XCode to the latest version (using the AppStore)
3) Open the file app/QtFPSGui/QtFPSGui.pro in QtCreator
4) Add/change the line
QMAKE_MAC_SDK = macosx10.11 (this is due to a weird error in latest Xcode versions)

or whatever version the command shows:
ls /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/

5) Build & run from within QtCreator
