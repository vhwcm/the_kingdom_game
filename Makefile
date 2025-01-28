TARGET=the_kingdom_game
CC=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
CURSES=-lncurses
TINFO=-ltinfo
CCFLAGS=$(DEBUG) $(OPT) $(WARN)
LD=g++
OBJS=the_kingdom.o the_kingdom_game.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(CURSES) $(TINFO)

the_kingdom.o: the_kingdom.cpp the_kingdom.h
	$(CC) $(CCFLAGS) -c the_kingdom.cpp

the_kingdom_game.o: the_kingdom_game.cpp the_kingdom.h
	$(CC) $(CCFLAGS) -c the_kingdom_game.cpp

clean:
	rm -f $(TARGET) $(OBJS)