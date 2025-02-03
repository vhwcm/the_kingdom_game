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

// Constants for the game
#define NUM_CARDS 54
#define NUM_NIPES 4
#define NUM_TYPES 13
#define NUM_START_HAND 3
#define CARD_HEIGHT 7
#define HALF_CARD_WIDTH 9
#define CARD_WIDTH 15
#define FULL_CARD_HEIGTH 10

// Card values and suits
const std::string card_values[] = {
    "ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING", "JOKER"};

#define HEARTS 0
#define DIAMONDS 1
#define CLUBS 2
#define SPADES 3
#define JOKER 4

const std::string card_suits[] = {
    "HEARTS", "DIAMONDS", "CLUBS", "SPADES", "JOKER"};

// Card class representing a card with a suit and a value
class Card
{
public:
    int nipe;
    int number;
    Card(int n = 0, int num = 0) : nipe(n), number(num) {}
    bool operator==(const Card &other) const
    {
        return (nipe == other.nipe && number == other.number);
    }
};

// Coord class representing coordinates on the screen
class Coord
{
public:
    Coord();                    // Constructor to initialize coordinates
    void set(int y, int x);     // Set coordinates to (y, x)
    void set();                 // Set coordinates to current values
    void down(int y = 1);       // Move down by y units
    void up(int y = 1);         // Move up by y units
    void left(int x = 1);       // Move left by x units
    void right(int x = 1);      // Move right by x units
    std::pair<int, int> show(); // Show current coordinates

private:
    std::pair<int, int> yx; // Pair to store coordinates
};

// Deck class representing a deck of cards
class Deck
{
private:
    int qnt;                 // Quantity of cards
    std::vector<Card> cards; // Vector of cards

public:
    Deck();                  // Constructor to initialize the deck
    void shuffDeck();        // Shuffle the deck
    Card drawnCard();        // Draw a card from the deck
    void draw(Coord &coord); // Draw the deck on the screen
};

// DiamondBank class representing a bank of diamond cards
class DiamondBank
{
private:
    std::vector<Card> diamonds_cards; // Vector of diamond cards
    std::vector<Card> shield_cards;   // Vector of shield cards

public:
    DiamondBank(std::vector<Card>);           // Constructor to initialize the diamond bank
    void draw(Coord &coord);                  // Draw the diamond bank on the screen
    int attacked(std::vector<Card>, int pos); // Handle attack on the diamond bank
    void addDiamondCard(std::vector<Card>);   // Add cards to the diamond bank
    void addShieldCard(std::vector<Card>);
    int numCards();
    bool full();
};

// Hand class representing a player's hand of cards
class Hand
{
private:
    int qnt;                 // Quantity of cards
    std::vector<Card> cards; // Vector of cards

public:
    Hand();                 // Constructor to initialize the hand
    Card getCard(int num);  // Get a card from the hand
    int num();              // Get the number of cards in the hand
    void showCards() const; // Show the cards in the hand
    void addCard(Card);     // Add a card to the hand
    void removeCard(Card);
};

// Player class representing a player in the game
class Player
{
private:
    int id; // Player ID

public:
    Player(int num_id);         // Constructor to initialize the player
    Deck deck;                  // Player's deck
    Hand hand;                  // Player's hand
    int drawnFromDeck(int num); // Draw cards from the deck
    int getId();
};

// Board class representing the game board
class Board
{
private:
    int p1Life;                              // Player 1's life
    int p2Life;                              // Player 2's life
    std::vector<Card> p1Warriors;            // Player 1's warriors
    std::vector<Card> p2Warriors;            // Player 2's warriors
    std::vector<DiamondBank> p1DiamondBanks; // Player 1's diamond banks
    std::vector<DiamondBank> p2DiamondBanks; // Player 2's diamond banks

public:
    Board();                                                   // Constructor to initialize the board
    int addWarrior(int life, int player);                      // Add a warrior to the board
    int addShield(int pos, int shield, int player);            // Add a shield to a warrior
    int breakShield(int pos, int player);                      // Break a shield of a warrior
    int atackWarrior(int pos, int atack, int player);          // Attack a warrior
    int executeWarrior(int pos, int player);                   // Execute a warrior
    int tradeWarrior(int pos_ally, int pos_enemy, int player); // Trade warriors between players
    void draw(Player &p1, Player &p2);                         // Draw the board on the screen
    void printEnemyCards(Player &p2, Coord &coord);            // Print enemy's cards
    void printPlayerCards(Player &p1, Coord &coord);           // Print player's cards
    void printDiamondBanks(Player &p, Coord &coord);           // Print the gold bank
    void printWarriorsOnBoard();
};

// Function declarations for general functions and networking
void showMessage(const char *message);              // Show a message on the screen
void printEnemyFullCardBack(Coord &coord, int num); // Print the back of an enemy's full card
void printEnemyHalfCardBack(Coord &coord, int num); // Print the back of an enemy's half card
int getint();                                       // Get an integer input from the user

void serverMode(int port);                               // Start the server mode
void clientMode(const std::string &server_ip, int port); // Start the client mode
void handleConnection(int socket);                       // Handle the connection
void printTitle(Coord &myCoord);                         // Print the game title
int printOptions(Coord &myCoord);                        // Print the game options
void botGame();                                          // Start the bot game mode
void onlineGame(Coord &myCoord);                         // Start the online game mode
void gameInstructions();                                 // Show the game instructions
void printQuit(Coord &myCoord);                          // Print the quit message
void showMessage(const char *message);                   // Show a message on the screen
void printEnemyFullCardBack(Coord &coord, int num);      // Print the back of an enemy's full card
void printEnemyHalfCardBack(Coord &coord, int num);      // Print the back of an enemy's half card
void printHalfCard(Coord &coord, Card card, int num);    // Print the front of a half card
void printFullCard(Coord &coord, Card card, int num);    // Print the front of a full card
void quit();                                             // Quit the game

void heartsCard(bool player_time, Board &board, Player &p1, Player &p2);   // Handle hearts cards (warriors)
void diamondsCard(bool player_time, Board &board, Player &p1, Player &p2); // Handle diamonds cards (gold)
void clubsCard(bool player_time, Board &board, Player &p1, Player &p2);    // Handle clubs cards (shields)
void spadesCard(bool player_time, Board &board, Player &p1, Player &p2);   // Handle spades cards (attacks)
void jokerCard(bool player_time, Board &board, Player &p1, Player &p2);    // Handle joker cards

#endif // THE_KINGDOM_H