#ifndef THE_KINGDOM_H
#define THE_KINGDOM_H

#include <map>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <utility>
#include <ncurses.h>

#define NUM_CARDS 54
#define NUM_NIPES 4
#define NUM_TYPES 13
#define NUM_START_HAND 3

const std::string card_values[] = {
    "ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING", "JOKER"
};

const std::string card_suits[] = {
    "HEARTS", "DIAMONDS", "CLUBS", "SPADES", "JOKER_SUIT"
};

class Coord {
public:
    Coord();
    void set(int y, int x);
    void set();
    void down();
    void top();
    void left();
    void right();
    std::pair<int, int> show();

private:
    std::pair<int, int> yx;
};

class Deck {
private:
    int qnt;
    std::vector<std::pair<int, int>> cards;

public:
    Deck();
    void shuffDeck();
    std::pair<int, int> drawCard();
};

class Hand {
private:
    int qnt;
    std::vector<std::pair<int, int>> cards;

public:
    void setHand(Deck& deck);
    void showCards() const;
    void drawFromDeck(Deck& deck);
};

void serverMode(int port);
void clientMode(const std::string& server_ip, int port);
void handleConnection(int socket);
void printTitle(Coord& myCoord);
void printOptions(Coord& myCoord);
void botGame(Coord& myCoord);
void onlineGame(Coord& myCoord);
void gameInstructions();
void printQuit(Coord& myCoord);
void quit();
int getint();

#endif // THE_KINGDOM_H