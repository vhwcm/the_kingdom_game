#include "the_kingdom.h"

Hand::Hand()
{
    qnt = 0;
}

Card Hand::getCard(int num)
{
    return cards[num];
}

int Hand::num()
{
    return qnt;
}

void Hand::showCards() const
{
    std::cout << "Your Hand:" << std::endl;
    for (const auto &card : cards)
    {
        std::cout << card_values[card.nipe] << " of " << card_suits[card.number] << std::endl;
    }
}

void Hand::addCard(Card card)
{
    cards.push_back(card);
    qnt++;
}
