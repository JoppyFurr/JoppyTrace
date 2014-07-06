#!/bin/sh

gcc *.c -o JoppyTrace -lm -lSDL2 -DJT_VERSION=\"`date --rfc-3339=date`\"
