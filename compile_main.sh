#!/bin/bash
gcc -g -O0 link.c conf.c pipe.c netdevs.c main.c -o main $(pkg-config --cflags --libs libnl-3.0) -Wl,-rpath=/usr/lib/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu -l:libnl-route-3.so.200

