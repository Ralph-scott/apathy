#!/bin/sh

set -xe

CC="gcc"
NAME="apathy"
CFLAGS="-Wall -Wextra -pedantic"
LDFLAGS="-lncurses -ltinfo -lm"

$CC $CFLAGS $LDFLAGS -o $NAME src/*.c