#!/bin/bash

cd build
qmake ../QtFPSConsoleRpi
make -j 2
cd ..