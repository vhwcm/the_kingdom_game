#include "the_kingdom.h"

Deck::Deck()
{
    qnt = NUM_CARDS;
    for (short nipe = 0; nipe < NUM_NIPES; nipe++)
    {
        for (short number = 1; number <= NUM_TYPES; number++)
        {
            cards.push_back(Card(nipe, number));
        }
    }
    cards.push_back(Card(4, 13));
    cards.push_back(Card(4, 13));
}

void Deck::shuffDeck()
{
    std::random_device rd;
    std::mt19937 g(rd());
    for (size_t i = 0; i < cards.size(); ++i)
    {
        std::uniform_int_distribution<> distrib(i, cards.size() - 1);
        std::swap(cards[i], cards[distrib(g)]);
    }
}

Card Deck::drawnCard()
{
    if (cards.empty())
    {
        throw std::out_of_range("There aren't more cards in the deck");
    }
    else
    {
        Card card = cards.back();
        cards.pop_back();
        qnt--;
        return card;
    }
}

void Deck::draw(Coord &coord)
{
    printw("######DECK######");
    coord.down();
    for (int i = 0; i < FULL_CARD_HEIGTH; i++)
    {
        if (i == FULL_CARD_HEIGTH / 2)
        {
            printw("#      %2i      #", qnt);
        }
        else if (i == FULL_CARD_HEIGTH / 2 + 1)
        {
            printw("#Card(s) Remain#");
        }
        else
        {
            printw("#              #");
        }
        coord.down();
    }
    printw("################");
}
