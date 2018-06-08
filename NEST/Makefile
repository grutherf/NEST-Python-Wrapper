#
# Makefile for LibNEST
#
#	Jacob Cutter, 3/14/2018 (Pi Day!)

CC=g++
# Note that DEBUG=-g produces weird .dYSM folder on Macs
#DEBUG=-Wall -g
DEBUG=
OPTIMIZATION=-O3

# OS Settings
OS := $(shell uname)
HOST := $(shell hostname | cut -c 1-4)
CURRENT_DIR=$(shell pwd)
# For Mac
ifeq ($(OS),Darwin)
LINKFLAG =-shared -dynamiclib
OBJLINKFLAG =
# Else (Linux)
else
LINKFLAG =-fPIC -shared
OBJLINKFLAG =-fPIC
endif

CFLAGS=-std=c++11 -c $(DEBUG) $(OPTIMIZATION)
CFLAGS_SO=-std=c++11 $(DEBUG) $(OPTIMIZATION)

BIN = testNEST
LIB = libNEST.so
OBJECTS = obj/RandomGen.o obj/TestSpectra.o obj/VDetector.o
HEADERS = NEST.hh $(wildcard Detectors/*.hh) 

################################################################################
all: $(BIN) $(LIB)

clean:
	rm -f $(BIN) obj/*.o *.so
################################################################################

$(BIN): $(BIN).cpp obj/NEST.o $(OBJECTS)
	$(CC) $^ $(CFLAGS_SO) -o $@ 

$(LIB): obj/NEST.o $(OBJECTS)
	$(CC) $^ $(CFLAGS_SO) $(LINKFLAG) -o $@

obj/NEST.o: NEST.cpp $(HEADERS) $(OBJECTS)
	$(CC) $< $(CFLAGS) $(OBJLINKFLAG) -o $@

$(OBJECTS): obj/%.o : %.cpp $(HEADERS)

obj/%.o: %.cpp $(HEADERS)
	$(CC) $< $(CFLAGS) $(OBJLINKFLAG) -o $@

