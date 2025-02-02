#include "the_kingdom.h"

Player::Player(int num_id)
{
    id = num_id;
}

int Player::getId()
{
    return id;
}

int Player::drawnFromDeck(int num)
{
    while (num--)
    {
        try
        {
            Card card = deck.drawnCard();
            hand.addCard(card);
            // Add the drawn card to the player's hand or perform other actions
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }
    }
    return 1;
}
