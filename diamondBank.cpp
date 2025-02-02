#include "the_kingdom.h"

DiamondBank::DiamondBank(std::vector<Card> cards)
{
    addDiamondCard(cards);
}

void DiamondBank::draw(Coord &coord)
{
    if (diamonds_cards.size() <= 0)
        return;

    int i;
    for (i = 0; i < (int)diamonds_cards.size() - 1; i++)
    {
        std::cout << "\033[33m"; // Set text color to yellow
        printHalfCard(coord, diamonds_cards[i], i);
        std::cout << "\033[0m"; // Reset text color to normal
        coord.up(CARD_HEIGHT);
        coord.right(HALF_CARD_WIDTH);
    }
    if (shield_cards.size())
    {
        std::cout << "\033[33m"; // Set text color to yellow
        printHalfCard(coord, diamonds_cards[i], i);
        std::cout << "\033[0m"; // Reset text color to normal
        coord.up(CARD_HEIGHT);
        coord.right(HALF_CARD_WIDTH);
        for (i = 0; i < (int)shield_cards.size() - 1; i++)
        {
            coord.up(CARD_HEIGHT);
            coord.right(HALF_CARD_WIDTH);
            std::cout << "\033[33m"; // Set text color to yellow
            printHalfCard(coord, shield_cards[i], i);
            std::cout << "\033[0m"; // Reset text color to normal
        }

        std::cout << "\033[33m"; // Set text color to yellow
        printFullCard(coord, shield_cards[i], i);
        std::cout << "\033[0m"; // Reset text color to normal
    }
    else
    {
        std::cout << "\033[33m"; // Set text color to yellow
        printFullCard(coord, diamonds_cards[i], i);
        std::cout << "\033[0m"; // Reset text color to normal
    }
}

int DiamondBank::attacked(std::vector<Card> atack, int pos)
{
    if (pos < 0)
        return -1;
    if (shield_cards.size())
    {
        if (pos >= (int)shield_cards.size())
            return -1;

        else
        {
            int value_atack = 0;
            for (auto card : atack)
                value_atack += card.number;
            if (value_atack < shield_cards[pos].number)
                return 1;
            else
            {
                shield_cards.erase(shield_cards.begin() + pos);
                return 0;
            }
        }
    }
    else
    {
        if (pos >= (int)diamonds_cards.size())
            return -1;

        else
        {
            int value_atack = 0;
            for (auto card : atack)
                value_atack += card.number;
            if (value_atack < diamonds_cards[pos].number)
                return 1;
            else
            {
                diamonds_cards.erase(shield_cards.begin() + pos);
                return 0;
            }
        }
    }
}

void DiamondBank::addDiamondCard(std::vector<Card> cards)
{
    for (auto card : cards)
        diamonds_cards.push_back(card);
}
void DiamondBank::addShieldCard(std::vector<Card> cards)
{
    for (auto card : cards)
        shield_cards.push_back(card);
}

int DiamondBank::numCards()
{
    return (int)(diamonds_cards.size() + shield_cards.size());
}

bool DiamondBank::full()
{
    int acmul = 0;
    for (auto card : diamonds_cards)
    {
        acmul += card.number;
    }

    if (acmul >= 15)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}