#!/bin/bash
#gcc -std=c11 -shared -Wl,-install_name,testlib.so -o testlib.so -fPIC testlib.c
g++-6 -g -std=c11 -Wall -Wpedantic testlib.c -o testlib.exe
./testlib.exe
#python testlibwrapper.py
