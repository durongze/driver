#!/bin/bash

find ./ -iname "Makefile" | xargs -I {}  sed '/.*KERNELDIR=/{n;/EXTRA_FLAGS=/ba;s!.*!EXTRA_FLAGS=-Dsimple_DEBUG\n&!;:a}' -i {} 
