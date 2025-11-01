#!/bin/bash

make clean
make
cd bin
#notify-send "DunGen started!"
./DunGen
