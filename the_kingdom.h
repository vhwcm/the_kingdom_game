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
#define CARD_HEIGHT 7
#define HALF_CARD_WIDTH 9

const std::string card_values[] = {
    "ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING", "JOKER"};

const std::string card_suits[] = {
    "HEARTS", "DIAMONDS", "CLUBS", "SPADES", "JOKER_SUIT"};

class Coord
{
public:
    Coord();
    void set(int y, int x);
    void set();
    void down(int y = 1);
    void top(int y = 1);
    void left(int x = 1);
    void right(int x = 1);
    std::pair<int, int> show();

private:
    std::pair<int, int> yx;
};

class Deck
{
private:
    int qnt;
    std::vector<std::pair<int, int>> cards;

public:
    Deck();
    void shuffDeck();
    std::pair<int, int> drawnCard();
};

class Hand
{
private:
    int qnt;
    std::vector<std::pair<int, int>> cards;

public:
    Hand();
    int num();
    void showCards() const;
    void addCard(std::pair<int, int> card);
};

class Player
{
public:
    Deck deck;
    Hand hand;
    int drawnFromDeck(int num);
};

class Board
{
private:
    int p1Life;
    int p2Life;
    std::vector<std::pair<int, int>> p1Warriors;
    std::vector<std::pair<int, int>> p2Warriors;

public:
    Board();
    int addWarrior(int life, int player);
    int addShield(int pos, int shield, int player);
    int breakShield(int pos, int player);
    int atackWarrior(int pos, int atack, int player);
    int executeWarrior(int pos, int player);
    int tradeWarrior(int pos_ally, int pos_enemy, int player);
    void draw(Player &p1, Player &p2, Coord &coord);
};

void showMessage(const char *message);
void printEnemyFullCardBack(Coord &coord, int num);
void printEnemyHalfCardBack(Coord &coord, int num);
int getint();

void serverMode(int port);
void clientMode(const std::string &server_ip, int port);
void handleConnection(int socket);
void printTitle(Coord &myCoord);
int printOptions(Coord &myCoord);
void botGame();
void onlineGame(Coord &myCoord);
void gameInstructions();
void printQuit(Coord &myCoord);
void quit();

#endif // THE_KINGDOM_H