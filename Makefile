CC=g++
CFLAGS=-I.
DEPS = globals.h bird.h speedy.h canon.h block.h
OBJ = io.o handlers.o misc.o parent_exec.o built_in.o jobs.o pipes.o shell.o

all: main

main: main.cpp glad.c
	g++ -w -DLIBAuDIO -o main main.cpp glad.c -lalut -lGL -lglfw -lftgl -lopenal -lSOIL -ldl -I/usr/local/include -I/usr/local/include/freetype2 -L/usr/local/lib

clean:
	rm main
