#!/bin/bash
gcc -O0 -g concat.c ip/netdevs.c conf.c pipe.c main.c -o main
gcc stdreaper.c -o stdreaper
