#
# Makefile for LibNEST
#
#	Jacob Cutter, 3/14/2018 (Pi Day!)

CC=g++
# Note that DEBUG=-g produces weird .dYSM folder on Macs
#DEBUG=-Wall -g
DEBUG=
OPTIMIZATION=-O3

CFLAGS=-std=c++11 -c $(DEBUG) $(OPTIMIZATION)
CFLAGS_SO=-std=c++11 -O3 $(DEBUG)

BIN = testNEST
OBJECTS = obj/NEST.o obj/TestSpectra.o 
UTILS = .

################################################################################
all: $(BIN)

clean:
	rm -f $(BIN) obj/*.o 
################################################################################

$(BIN): obj/$(BIN).o $(OBJECTS)
	$(CC) $(LIBS) -o $@ obj/$(BIN).o $(OBJECTS)

obj/$(BIN).o: $(BIN).cpp
	$(CC) $< -c $(CFLAGS) -o $@

$(OBJECTS): obj/%.o : $(UTILS)/%.cpp $(UTILS)/%.hh

obj/%.o: $(UTILS)/%.cpp $(UTILS)/%.hh
	$(CC) $< -c $(CFLAGS) -o $@

