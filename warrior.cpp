#include "the_kingdom.h"

// New constructor to initialize warrior with no shield
Warrior::Warrior(Card card) : warrior(card)
{
    if (card.nipe != HEARTS)
    {
        showMessage("Error: Card is not a heart card.");
        // Destruct the class
        this->~Warrior();
        return;
    }
    shield.nipe = -1; // indica que não há escudo
    shield.number = 0;
}

int Warrior::addShield(Card shieldCard)
{
    // Se já há escudo, retorna código de erro
    if (shield.nipe != -1)
    {
        return 2;
    }
    shield = shieldCard;
    return 0;
}

int Warrior::atack(int attack)
{
    // Se há escudo (neste exemplo, nipe diferente de -1 indica presença de escudo)
    if (shield.nipe != -1 && shield.number > 0)
    {
        if (attack < shield.number)
            return -2; // dano insuficiente para quebrar o escudo
        else
        {
            // Quebra o escudo
            shield.nipe = -1;
            shield.number = 0;
            return 1; // escudo quebrado
        }
    }
    else
    {
        // Sem escudo, utiliza o número da carta warrior como vida
        if (attack < warrior.number)
            return -3; // dano insuficiente para matar o guerreiro
        else
            return 2; // guerreiro morto
    }
}

std::pair<int, int> Warrior::stats() const
{
    return std::make_pair(warrior.number, shield.number);
}

void Warrior::draw(Coord &coord, int num) const
{
    if (shield.number == 0)
    {
        printFullFullCard(coord, warrior, num);
        coord.right(CARD_WIDTH);
    }
    else
    {
        printFullHalfCard(coord, warrior, num);
        coord.up(FULL_CARD_HEIGTH);
        coord.right(HALF_CARD_WIDTH);
        printFullFullCard(coord, shield, num);
        coord.right(CARD_WIDTH);
    }
}
