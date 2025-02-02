TARGET=the_kingdom_game
CC=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
CURSES=-lncurses
TINFO=-ltinfo
CCFLAGS=$(DEBUG) $(OPT) $(WARN)
LD=g++
OBJS=the_kingdom.o the_kingdom_game.o coord.o deck.o hand.o player.o board.o diamondBank.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(CURSES) $(TINFO)

the_kingdom.o: the_kingdom.cpp the_kingdom.h
	$(CC) $(CCFLAGS) -c the_kingdom.cpp

the_kingdom_game.o: the_kingdom_game.cpp the_kingdom.h
	$(CC) $(CCFLAGS) -c the_kingdom_game.cpp

coord.o: coord.cpp the_kingdom.h
	$(CC) $(CCFLAGS) -c coord.cpp

deck.o: deck.cpp the_kingdom.h
	$(CC) $(CCFLAGS) -c deck.cpp

hand.o: hand.cpp the_kingdom.h
	$(CC) $(CCFLAGS) -c hand.cpp

player.o: player.cpp the_kingdom.h
	$(CC) $(CCFLAGS) -c player.cpp

board.o: board.cpp the_kingdom.h
	$(CC) $(CCFLAGS) -c board.cpp

diamondBank.o: diamondBank.cpp the_kingdom.h
	$(CC) $(CCFLAGS) -c diamondBank.cpp
clean:
	rm -f $(TARGET) $(OBJS)