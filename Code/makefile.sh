#!/bin/bash

CC = gcc
CC_FLAGS = -g -Wall -ansi -pedantic
OUT_EXE = main
FILES = main.o

build: $(FILES)
	$(CC) $(CC_FLAGS) -o $(OUT_EXE) $(FILES)
clean:
	rm -rf $(OUT_EXE) *.o *.exe
