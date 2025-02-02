#include "the_kingdom.h"

DiamondBank::DiamondBank(std::vector<Card>)
{
}

void DiamondBank::draw(Coord &coord)
{
    if (diamonds_cards.size() <= 0)
        return;

    int i;
    for (i = 0; i < (int)diamonds_cards.size() - 1; i++)
    {
        printHalfCard(coord, diamonds_cards[i], i);
    }
    if (shield_cards.size())
    {
        printHalfCard(coord, diamonds_cards[i], i);
    }
    else
    {
        printFullCard(coord, diamonds_cards[i], i);
    }
    for (i = 0; i < (int)shield_cards.size() - 1; i++)
    {
        printHalfCard(coord, shield_cards[i], i);
    }
    if (shield_cards.size())
    {
        printFullCard(coord, shield_cards[i], i);
    }
}