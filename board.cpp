#include "the_kingdom.h"

Board::Board()
{
    p1Life = 0;
    p1Life = 0;
}

int Board::addWarrior(int life, int player)
{
    if (player == 1)
    {
        p1Warriors.push_back(Card(life, 0));
        p1Life += life;
        return 0;
    }
    if (player == 2)
    {
        p2Warriors.push_back(Card(life, 0));
        p1Life += life;
        return 0;
    }
    return -1; // player invalido
}

int Board::addShield(int pos, int shield, int player)
{
    if (player == 1)
    {
        if (pos < 0 || pos > (int)p1Warriors.size() - 1)
            return 1; // Carta inexistente!
        if (p1Warriors[pos].number != 0)
            return 2; // Carta ja tem um escudo

        p1Warriors[pos].number = shield;
        return 0;
    }
    else if (player == 2)
    {
        if (pos < 0 || pos > (int)p2Warriors.size() - 1)
            return 1; // Carta inexistente!
        if (p2Warriors[pos].number != 0)
            return 2; // Carta ja tem um escudo

        p2Warriors[pos].number = shield;
        return 0;
    }
    else
    {
        return -1; // player invalido
    }
}

int Board::breakShield(int pos, int player)
{
    if (player == 1)
    {
        if (pos < 0 || pos > (int)p1Warriors.size() - 1)
            return 1; // Carta inexistente!
        if (p1Warriors[pos].number == 0)
            return 2; // Carta ja tem um escudo

        p1Warriors[pos].number = 0;
        return 0;
    }
    else if (player == 2)
    {
        if (pos < 0 || pos > (int)p2Warriors.size() - 1)
            return 1; // Carta inexistente!
        if (p2Warriors[pos].number == 0)
            return 2; // Carta ja tem um escudo

        p2Warriors[pos].number = 0;
        return 0;
    }
    else
    {
        return -1; // player invalido
    }
}

int Board::atackWarrior(int pos, int atack, int player)
{
    if (player == 1)
    {
        if (pos < 0 || pos > (int)p2Warriors.size() - 1)
            return -1; // Carta inexistente
        if (p2Warriors[pos].number != 0)
        {
            if (atack < p2Warriors[pos].number)
                return -2;
            else
            {
                p2Warriors[pos].number = 0;
                return 1;
            }
        }
        else
        {
            if (atack < p2Warriors[pos].nipe)
                return -3;
            else
            {
                p1Life -= p2Warriors[pos].nipe;
                p2Warriors.erase(p2Warriors.begin() + pos);
                return 2;
            }
        }
    }
    else if (player == 2)
    {
        if (pos < 0 || pos > (int)p1Warriors.size() - 1)
            return -1; // Carta inexistente
        if (p1Warriors[pos].number != 0)
        {
            if (atack < p1Warriors[pos].number)
                return -2;
            else
            {
                p1Warriors[pos].number = 0;
                return 1;
            }
        }
        else
        {
            if (atack < p1Warriors[pos].nipe)
                return -3;
            else
            {
                p1Life -= p1Warriors[pos].nipe;
                p1Warriors.erase(p1Warriors.begin() + pos);
                return 2;
            }
        }
    }
    else
    {
        return -4; // player invalido
    }
}

int Board::executeWarrior(int pos, int player)
{
    if (player == 1)
    {
        if (pos < 0 || pos > (int)p2Warriors.size() - 1)
            return 1;
        p1Life -= p2Warriors[pos].nipe;
        p2Warriors.erase(p2Warriors.begin() + pos);
        return 0;
    }
    else if (player == 2)
    {
        if (pos < 0 || pos > (int)p1Warriors.size() - 1)
            return 1;
        p1Life -= p1Warriors[pos].nipe;
        p1Warriors.erase(p1Warriors.begin() + pos);
        return 0;
    }
    else
        return -1;
}

int Board::tradeWarrior(int pos_ally, int pos_enemy, int player)
{
    if (player == 1)
    {
        if (pos_ally < 0 || pos_ally > (int)p1Warriors.size() - 1 || pos_enemy < 0 || pos_enemy > (int)p2Warriors.size() - 1)
        {
            return 1;
        }

        p1Life -= p1Warriors[pos_ally].nipe;
        p1Life -= p2Warriors[pos_enemy].nipe;

        p1Life += p2Warriors[pos_enemy].nipe;
        p1Life += p1Warriors[pos_ally].nipe;

        Card aux = p1Warriors[pos_ally];
        p1Warriors.erase(p1Warriors.begin() + pos_ally);
        p2Warriors.push_back(aux);

        aux = p2Warriors[pos_enemy];
        p2Warriors.erase(p2Warriors.begin() + pos_enemy);
        p1Warriors.push_back(aux);
        return 0;
    }

    if (player == 2)
    {
        if (pos_ally < 0 || pos_ally > (int)p2Warriors.size() - 1 || pos_enemy < 0 || pos_enemy > (int)p1Warriors.size() - 1)
        {
            return 1;
        }

        p1Life -= p2Warriors[pos_ally].nipe;
        p1Life -= p1Warriors[pos_enemy].nipe;

        p1Life += p1Warriors[pos_enemy].nipe;
        p1Life += p2Warriors[pos_ally].nipe;

        Card aux = p2Warriors[pos_ally];
        p2Warriors.erase(p2Warriors.begin() + pos_ally);
        p1Warriors.push_back(aux);

        aux = p1Warriors[pos_enemy];
        p1Warriors.erase(p1Warriors.begin() + pos_enemy);
        p2Warriors.push_back(aux);
        return 0;
    }

    return -1; // player invalido
}

void Board::draw(Player &p1, Player &p2, Coord &coord)
{
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    printEnemyCards(p2, coord);
    coord.set(max_y - (CARD_HEIGHT + 2), max_x - (CARD_WIDTH + 2));
    printPlayerCards(p1, coord);
    coord.set(max_y - (FULL_CARD_HEIGTH + 2), 2);
    p1.deck.draw(coord);
    coord.set(0, max_x - (CARD_WIDTH + 2));
    p2.deck.draw(coord);
    refresh();
}

void Board::printGoldBank(Player &p, Coord &coord)
{
    if (p.getId() == 1)
    {
    }
    else
    {
    }
}

void Board::printEnemyCards(Player &p2, Coord &coord)
{
    int i;
    for (i = 0; i < p2.hand.num() - 1; i++)
    {
        printEnemyHalfCardBack(coord, i);
        coord.top(CARD_HEIGHT);
        coord.right(HALF_CARD_WIDTH);
    }
    if (p2.hand.num())
        printEnemyFullCardBack(coord, i);
}

void Board::printPlayerCards(Player &p1, Coord &coord)
{
    int i = 0;
    if (p1.hand.num())
        printFullCard(coord, p1.hand.getCard(i), i);
    for (i = 1; i < p1.hand.num(); i++)
    {
        coord.top(CARD_HEIGHT);
        coord.left(HALF_CARD_WIDTH);
        printHalfCard(coord, p1.hand.getCard(i), i);
    }
}
