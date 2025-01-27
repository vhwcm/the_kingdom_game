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

class Coord{
public:
    Coord() {
        yx = std::make_pair(0,0);
    }
    
    void set(int y, int x) {
        yx = std::make_pair(y,x);
        move(yx.first,yx.second);
    }

    void set() {
        set(yx.first, yx.second);
    }

    void down() {
        yx.first += 1;  
        move(yx.first,yx.second);
    }

    void top() {
        yx.first -= 1;
        move(yx.first,yx.second);
    }

    void left() {
        yx.second -= 1;
        move(yx.first,yx.second);
    }

    void rigth() {
        yx.second += 1;
        move(yx.first,yx.second);
    }

    std::pair<int,int> show(){
        return yx;
    }
private:
    std::pair<int,int> yx;
};

class Deck{
private:
    int qnt;
    std::vector<std::pair<int,int>> cards;

public:
    Deck() {
        qnt = NUM_CARDS;
        for(short i = 0; i < NUM_NIPES; i++){
            for(short j = 0; j < NUM_TYPES;j++){
                cards.push_back(std::make_pair(i,j));
            }
        cards.push_back(std::make_pair(4,13));
        cards.push_back(std::make_pair(4,13));
        }
    }

    void shuffDeck(){
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(),g);
    }

    std::pair<int,int> drawCard(){
        if (cards.empty()) {
            throw std::out_of_range("No more cards in the deck");
        }
        std::pair<int,int> top = cards.front();
        cards.pop_back();
        qnt--;
        return top;
    }
};

// class Board{
//     private
// };


class Hand{
private:
    int qnt;
    std::vector<std::pair<int,int>> cards;
public:
    void setHand(Deck& deck) {
        qnt = 0;
        for(int i; i < NUM_START_HAND;i++)
            drawFromDeck(deck);
    }

    void showCards() const {
        std::cout << "Your Hand:" << std::endl;
        int i = 0;
        for (const auto& card : cards){
            std::cout << card_values[card.first] << " of " << card_suits[card.second] << std::endl;
        }
    }
    void drawFromDeck(Deck& deck) {
        try {
            std::pair<int, int> card = deck.drawCard();
            cards.push_back(card);
            qnt++;
        } catch (const std::out_of_range& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};


#endif // THE_KINGDOM_H