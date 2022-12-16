
#OBJ=obj/
#SRC=src
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(SRC_DIR)/main

all: main
james: src/main.c
	echo $< $@

main: $(SRC)/main.c records.o records.h
	gcc -o main main.c records.o

records.o: records.c records.h
	gcc -c records.c

.PHONY: rm
rm:
	rm -f records.o main