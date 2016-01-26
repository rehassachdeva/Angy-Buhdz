CC=g++
CFLAGS=-I.
DEPS = globals.h bird.h speedy.h canon.h block.h
OBJ = io.o handlers.o misc.o parent_exec.o built_in.o jobs.o pipes.o shell.o

all: main

main: main.cpp glad.c
	g++ -o main main.cpp glad.c -lGL -lglfw -lftgl -lSOIL -ldl -I/usr/local/include -I/usr/local/include/freetype2 -L/usr/local/lib

clean:
	rm main
