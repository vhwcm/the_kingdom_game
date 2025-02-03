#include "the_kingdom.h"

// Initialize the game board with zero life points for both players
Board::Board()
{
    p1Life = 0;
    p1Life = 0;
}

// Add a warrior card to a player's army and update their life points
// Returns: 0 on success, -1 if invalid player
int Board::addWarrior(int life, int player)
{
    if (player == 0)
    {
        p1Warriors.push_back(Card(life, 0));
        p1Life += life;
        return 0;
    }
    if (player == 1)
    {
        p2Warriors.push_back(Card(life, 0));
        p1Life += life;
        return 0;
    }
    return -1; // Invalid player
}

// Add a shield to a warrior at specified position
// Returns: 0 on success, 1 if card doesn't exist, 2 if already has shield, -1 if invalid player
int Board::addShield(int pos, int shield, int player)
{
    if (player == 1)
    {
        if (pos < 0 || pos > (int)p1Warriors.size() - 1)
            return 1; // Card doesn't exist!
        if (p1Warriors[pos].number != 0)
            return 2; // Card already has shield

        p1Warriors[pos].number = shield;
        return 0;
    }
    else if (player == 2)
    {
        if (pos < 0 || pos > (int)p2Warriors.size() - 1)
            return 1; // Card doesn't exist!
        if (p2Warriors[pos].number != 0)
            return 2; // Card already has shield

        p2Warriors[pos].number = shield;
        return 0;
    }
    else
    {
        return -1; // Invalid player
    }
}

// Break a warrior's shield at specified position
// Returns: 0 on success, 1 if card doesn't exist, 2 if no shield to break, -1 if invalid player
int Board::breakShield(int pos, int player)
{
    if (player == 1)
    {
        if (pos < 0 || pos > (int)p1Warriors.size() - 1)
            return 1; // Card doesn't exist!
        if (p1Warriors[pos].number == 0)
            return 2; // No shield to break

        p1Warriors[pos].number = 0;
        return 0;
    }
    else if (player == 2)
    {
        if (pos < 0 || pos > (int)p2Warriors.size() - 1)
            return 1; // Card doesn't exist!
        if (p2Warriors[pos].number == 0)
            return 2; // No shield to break

        p2Warriors[pos].number = 0;
        return 0;
    }
    else
    {
        return -1; // Invalid player
    }
}

// Attack a warrior with specified attack value
// Returns:
// 1 on successful shield break
// 2 on successful warrior kill
// -1 if target doesn't exist
// -2 if attack too weak for shield
// -3 if attack too weak for warrior
// -4 if invalid player
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

// Execute (remove) a warrior from the board
// Returns: 0 on success, 1 if target doesn't exist, -1 if invalid player
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

// Trade warriors between players
// Returns: 0 on success, 1 if invalid positions, -1 if invalid player
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

// Draw the entire game board including all cards and decks
void Board::draw(Player &p1, Player &p2, Coord &coord)
{
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Draw enemy cards at top, player cards at bottom
    coord.set(1, 0);
    printEnemyCards(p2, coord);
    coord.set(max_y - (CARD_HEIGHT + 1), max_x - (CARD_WIDTH));
    printPlayerCards(p1, coord);

    // Draw decks for both players
    coord.set(max_y - (FULL_CARD_HEIGTH + 3), 2);
    p1.deck.draw(coord);
    coord.set(1, max_x - (CARD_WIDTH + 2));
    p2.deck.draw(coord);
    printDiamondBanks(p1, coord);
    printDiamondBanks(p2, coord);
    refresh();
}

// Print the diamond banks (special card collections) for a player
void Board::printDiamondBanks(Player &p, Coord &coord)
{
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int size_used = 0;

    // Different positioning for each player
    if (p.getId() == 1)
    {
        // Player 1's banks at bottom
        coord.set(max_y, max_x);
        coord.up(CARD_HEIGHT + 2);
        for (auto diamondBank : p1DiamondBanks)
        {
            // Position each bank with proper spacing
            coord.up(CARD_HEIGHT);
            int size_left = HALF_CARD_WIDTH * (diamondBank.numCards() - 1) + CARD_WIDTH;
            size_used += size_left + 2;
            coord.left(size_used);
            diamondBank.draw(coord);
        }
    }
    else if (p.getId() == 2)
    {
        // Player 2's banks at top
        coord.set(0, 0);
        coord.down(CARD_HEIGHT + 2);
        for (auto diamondBank : p2DiamondBanks)
        {
            // Desloca para baixo: distância de uma carta mais 2
            coord.down(CARD_HEIGHT + 2);
            int size_left = HALF_CARD_WIDTH * (diamondBank.numCards() - 1) + CARD_WIDTH;
            size_used += size_left + 2;
            coord.right(size_used);
            diamondBank.draw(coord);
        }
    }
}

// Print enemy cards face down
void Board::printEnemyCards(Player &p2, Coord &coord)
{
    int i;
    // Print half cards for all but the last one
    for (i = 0; i < p2.hand.num() - 1; i++)
    {
        printEnemyHalfCardBack(coord, i);
        coord.up(CARD_HEIGHT);
        coord.right(HALF_CARD_WIDTH);
    }
    // Print full card for the last one if exists
    if (p2.hand.num())
        printEnemyFullCardBack(coord, i);
}

// Print player's cards face up
void Board::printPlayerCards(Player &p1, Coord &coord)
{
    int i = 0;
    // Print first card as full card if exists
    if (p1.hand.num())
        printFullCard(coord, p1.hand.getCard(i), i);
    // Print remaining cards as half cards
    for (i = 1; i < p1.hand.num(); i++)
    {
        coord.up(CARD_HEIGHT);
        coord.left(HALF_CARD_WIDTH);
        printHalfCard(coord, p1.hand.getCard(i), i);
    }
}
