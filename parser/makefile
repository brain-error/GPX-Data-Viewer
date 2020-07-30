CC = gcc
CFLAGS = -g -Wall -std=c11 -lm -fpic `xml2-config --cflags --libs`
CPPFLAGS = -I./include -I/usr/include/libxml2 -I./src
TARGET_LIB = libgpxparse.so

SRC_DIR = ./src
INCL_DIR = ./include
BIN_DIR = ../

SRCS = src/*.c
HEAD = include/*.h

all: libgpxparse.so

libgpxparse.so: objects
	$(CC) $(CFLAGS) -shared -o $(BIN_DIR)/$(TARGET_LIB) GPXParser.o LinkedListAPI.o HelperFunctions.o -lxml2

objects: $(SRCS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(SRCS)  -lxml2

.PHONY: clean
clean:
	rm -f *.o $(BIN_DIR)/*.so
