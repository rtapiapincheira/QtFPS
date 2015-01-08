#!/bin/bash

mkdir build
cd build
qmake ../QtFPSConsoleRpi.pro
make -j 2
cd ..
