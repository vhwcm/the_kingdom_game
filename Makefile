# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS = -lncurses -ltinfo

# Target executable name
TARGET = the_kingdom_game

# Source files
SOURCES = the_kingdom_game.cpp \
          the_kingdom.cpp \
          board.cpp \
          warrior.cpp \
          deck.cpp \
          coord.cpp \
          player.cpp \
          diamondBank.cpp \
          hand.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET)

# Dependencies
the_kingdom_game.o: the_kingdom_game.cpp the_kingdom.h
the_kingdom.o: the_kingdom.cpp the_kingdom.h
board.o: board.cpp the_kingdom.h
warrior.o: warrior.cpp the_kingdom.h
deck.o: deck.cpp the_kingdom.h
coord.o: coord.cpp the_kingdom.h
player.o: player.cpp the_kingdom.h
diamondBank.o: diamondBank.cpp the_kingdom.h
hand.o: hand.cpp the_kingdom.h

.PHONY: all clean