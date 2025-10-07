#!/bin/bash

make clean
make
cd bin
./mini3D
notify-send "Mini started!" # thats cool
