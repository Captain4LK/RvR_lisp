#!/bin/sh

gcc -o lisp main.c ../../src/RvR_lisp.c -std=c99 -Wall -Wextra -pedantic -O3 -s -flto -lm
