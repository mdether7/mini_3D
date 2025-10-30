#!/bin/bash

make clean
make
cd bin
#notify-send "Mini started!"
./DunGen
