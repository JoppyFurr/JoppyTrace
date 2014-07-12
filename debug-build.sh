#!/bin/sh

gcc -ggdb *.c -o JoppyTrace -lm -lSDL2 -lpthread -DJT_VERSION=\"`date --rfc-3339=date`\" -std=c99
