#!/bin/bash

make clean
make
cd bin
notify-send "Mini started!" # That's cool
./mini3D
