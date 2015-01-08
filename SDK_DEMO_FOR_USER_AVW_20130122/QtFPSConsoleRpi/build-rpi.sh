#!/bin/bash

mkdir build
cd build
qmake ../QtFPSConsoleRpi
make -j 2
cd ..
