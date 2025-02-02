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
#define CARD_WIDTH 15
#define FULL_CARD_HEIGTH 10

const std::string card_values[] = {
    "ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING", "JOKER"};

const std::string card_suits[] = {
    "HEARTS", "DIAMONDS", "CLUBS", "SPADES", "JOKER"};

class Card
{
public:
    int nipe;
    int number;
    Card(int n = 0, int num = 0) : nipe(n), number(num) {}
};

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
    std::vector<Card> cards;

public:
    Deck();
    void shuffDeck();
    Card drawnCard();
    void draw(Coord &coord);
};

class DiamondBank
{
private:
    std::vector<Card> diamonds_cards;
    std::vector<Card> shield_cards;

public:
    DiamondBank(std::vector<Card>);
    void draw(Coord &coord);
    int attacked(std::vector<Card>);
    void addCard(std::vector<Card>);
};

class Hand
{
private:
    int qnt;
    std::vector<Card> cards;

public:
    Hand();
    Card getCard(int num);
    int num();
    void showCards() const;
    void addCard(Card card);
};

class Player
{
private:
    int id;

public:
    Player(int num_id);
    Deck deck;
    Hand hand;
    int drawnFromDeck(int num);
    int getId();
};

class Board
{
private:
    int p1Life;
    int p2Life;
    std::vector<Card> p1Warriors;
    std::vector<Card> p2Warriors;
    std::vector<Card> p1Warriors;
    std::vector<Card> p2Warriors;
    std::vector<DiamondBank> p1DiamondBanks;
    std::vector<DiamondBank> p2DiamondBanks;

public:
    Board();
    int addWarrior(int life, int player);
    int addShield(int pos, int shield, int player);
    int breakShield(int pos, int player);
    int atackWarrior(int pos, int atack, int player);
    int executeWarrior(int pos, int player);
    int tradeWarrior(int pos_ally, int pos_enemy, int player);
    void draw(Player &p1, Player &p2, Coord &coord);
    void printEnemyCards(Player &p2, Coord &coord);
    void printPlayerCards(Player &p1, Coord &coord);
    void printGoldBank(Player &p, Coord &coord);
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