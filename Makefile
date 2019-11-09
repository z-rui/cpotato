CC=gcc
CFLAGS=-Wall -g
CFLAGS+=-O2

all: player rmaster

player: player.o util.o
rmaster: rmaster.o util.o

player.o: player.c util.h potato.nb.h
rmaster.o: rmaster.c util.h potato.nb.h
util.o: util.c util.h
