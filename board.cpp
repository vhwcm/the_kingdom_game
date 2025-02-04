#include "the_kingdom.h"

// Initialize the game board with zero life points for both players
Board::Board()
{
    p1Life = 0;
    p2Life = 0;
}

// Modificar addWarrior para receber um Card e criar um Warrior
int Board::addWarrior(Card card, int player)
{
    if (player == 1)
    {
        p1Warriors.push_back(Warrior(card));
        p1Life += card.number;
        return 0;
    }
    else if (player == 2)
    {
        p2Warriors.push_back(Warrior(card));
        p2Life += card.number;
        return 0;
    }
    return -1;
}

// Modified addShield to receive a Card; expected clubs card (paus)
int Board::addShield(int pos, Card shieldCard, int player)
{
    // Validate that the card is a Clubs card
    if (shieldCard.nipe != CLUBS)
    {
        return -2; // Error: card is not a Clubs card
    }

    if (player == 1)
    {
        if (pos < 0 || pos >= (int)p1Warriors.size())
            return 1;                                 // Warrior doesn't exist
        return p1Warriors[pos].addShield(shieldCard); // return 2 if arleady have a shield
    }
    else if (player == 2)
    {
        if (pos < 0 || pos >= (int)p2Warriors.size())
            return 1;
        return p2Warriors[pos].addShield(shieldCard);
    }
    return -1; // Invalid player
}

// Modificar atackWarrior para usar Warrior::atack
// Agora a função recebe 'attack' como int, e usa o método atack do Warrior alvo.
int Board::atackWarrior(int pos, int attack, int player)
{
    int result = 0;
    if (player == 1)
    { // attacker: Player 1; target: Player 2
        if (pos < 0 || pos >= (int)p2Warriors.size())
            return -1; // Invalid target
        result = p2Warriors[pos].atack(attack);
        if (result == 2)
        {
            // Warrior killed – remove from board (optionally update life if needed)
            p2Warriors.erase(p2Warriors.begin() + pos);
        }
    }
    else if (player == 2)
    { // attacker: Player 2; target: Player 1
        if (pos < 0 || pos >= (int)p1Warriors.size())
            return -1;
        result = p1Warriors[pos].atack(attack);
        if (result == 2)
        {
            p1Warriors.erase(p1Warriors.begin() + pos);
        }
    }
    else
        return -1;
    return result;
}

// Execute (remove) a warrior from the board
// Returns: 0 on success, 1 if target doesn't exist, -1 if invalid player
int Board::executeWarrior(int pos, int player)
{
    if (player == 1)
    {
        if (pos < 0 || pos >= (int)p2Warriors.size())
            return 1;
        p2Warriors.erase(p2Warriors.begin() + pos);
        return 0;
    }
    else if (player == 2)
    {
        if (pos < 0 || pos >= (int)p1Warriors.size())
            return 1;
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
        if (pos_ally < 0 || pos_ally >= (int)p1Warriors.size() ||
            pos_enemy < 0 || pos_enemy >= (int)p2Warriors.size())
            return 1;
        std::swap(p1Warriors[pos_ally], p2Warriors[pos_enemy]);
        return 0;
    }
    else if (player == 2)
    {
        if (pos_ally < 0 || pos_ally >= (int)p2Warriors.size() ||
            pos_enemy < 0 || pos_enemy >= (int)p1Warriors.size())
            return 1;
        std::swap(p2Warriors[pos_ally], p1Warriors[pos_enemy]);
        return 0;
    }
    return -1; // player invalido
}

// New: Function to attack the opponent's DiamondBank
// attackCards: cards used for attack
int Board::attackDiamondBank(int bankIndex, std::vector<Card> attackCards, int player)
{
    if (player == 1)
    {
        if (bankIndex < 0 || bankIndex >= (int)p2DiamondBanks.size())
            return -1; // invalid index
        return p2DiamondBanks[bankIndex].attacked(attackCards, bankIndex);
    }
    else if (player == 2)
    {
        if (bankIndex < 0 || bankIndex >= (int)p1DiamondBanks.size())
            return -1;
        return p1DiamondBanks[bankIndex].attacked(attackCards, bankIndex);
    }
    return -1;
}

// Create a new diamond bank from a diamond card
int Board::createDiamondBank(Card diamondCard, int player)
{
    if (diamondCard.nipe != DIAMONDS)
        return -2; // Card is not a Diamond

    std::vector<Card> cards{diamondCard};
    DiamondBank bank(cards);

    if (player == 1)
    {
        p1DiamondBanks.push_back(bank);
        return 0;
    }
    else if (player == 2)
    {
        p2DiamondBanks.push_back(bank);
        return 0;
    }
    return -1; // Invalid player
}

// Add a diamond card to an existing diamond bank
int Board::addDiamondToBank(int bankPos, Card diamondCard, int player)
{
    if (diamondCard.nipe != DIAMONDS)
        return -2; // Not a diamond card

    if (player == 1)
    {
        if (bankPos < 0 || bankPos >= (int)p1DiamondBanks.size())
            return -1; // Invalid bank position
        if (p1DiamondBanks[bankPos].full())
            return -3; // Bank is full
        std::vector<Card> cards{diamondCard};
        p1DiamondBanks[bankPos].addDiamondCard(cards);
        return 0;
    }
    else if (player == 2)
    {
        if (bankPos < 0 || bankPos >= (int)p2DiamondBanks.size())
            return -1;
        if (p2DiamondBanks[bankPos].full())
            return -3;
        std::vector<Card> cards{diamondCard};
        p2DiamondBanks[bankPos].addDiamondCard(cards);
        return 0;
    }
    return -1; // Invalid player
}

// Add a shield (clubs) card to protect a diamond bank
int Board::addShieldToBank(int bankPos, Card shieldCard, int player)
{
    if (shieldCard.nipe != CLUBS)
        return -2; // Not a clubs card

    if (player == 1)
    {
        if (bankPos < 0 || bankPos >= (int)p1DiamondBanks.size())
            return -1; // Invalid bank position
        std::vector<Card> cards{shieldCard};
        p1DiamondBanks[bankPos].addShieldCard(cards);
        return 0;
    }
    else if (player == 2)
    {
        if (bankPos < 0 || bankPos >= (int)p2DiamondBanks.size())
            return -1;
        std::vector<Card> cards{shieldCard};
        p2DiamondBanks[bankPos].addShieldCard(cards);
        return 0;
    }
    return -1; // Invalid player
}

// Count how many diamond banks are full for a given player
int Board::countFullDiamondBanks(int player)
{
    int fullBanks = 0;

    if (player == 1)
    {
        for (const auto &bank : p1DiamondBanks)
        {
            if (bank.full())
            {
                fullBanks++;
            }
        }
    }
    else if (player == 2)
    {
        for (const auto &bank : p2DiamondBanks)
        {
            if (bank.full())
            {
                fullBanks++;
            }
        }
    }

    return fullBanks; // Returns 0 if invalid player or no full banks
}

// Draw the entire game board including all cards and decks
void Board::draw(Player &p1, Player &p2)
{
    Coord coord;
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
    printDiamondBanks(p1);
    printDiamondBanks(p2);
    printWarriorsOnBoard();
    refresh();
}

// Print the diamond banks (special card collections) for a player
void Board::printDiamondBanks(Player &p)
{
    Coord coord;
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

void Board::printWarriorsOnBoard()
{
    Coord coord;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    const int middle_y = max_y / 2;
    const int middle_x = max_x / 2;
    int p1_total_size = 0;
    int p2_total_size = 0;

    for (auto warrior : p1Warriors)
    {
        if (warrior.stats().second == 0)
        {
            p1_total_size += CARD_WIDTH + 1;
        }
        else
        {
            p1_total_size += CARD_WIDTH + HALF_CARD_WIDTH + 1;
        }
    }

    for (auto warrior : p2Warriors)
    {
        if (warrior.stats().second == 0)
        {
            p2_total_size += CARD_WIDTH + 1;
        }
        else
        {
            p2_total_size += CARD_WIDTH + HALF_CARD_WIDTH + 1;
        }
    }

    coord.set(middle_y, middle_x - (p1_total_size / 2));
    coord.down(FULL_CARD_HEIGTH + 1);
    for (const Warrior warrior : p1Warriors)
    {
        int index = 0;
        warrior.draw(coord, index);
        coord.up(FULL_CARD_HEIGTH);
        coord.right(2);
    }

    coord.set(middle_y, middle_x - (p2_total_size / 2));
    coord.up(FULL_CARD_HEIGTH + 1);
    for (const Warrior warrior : p2Warriors)
    {
        int index = 0;
        warrior.draw(coord, index);
        coord.up(FULL_CARD_HEIGTH);
        coord.right(2);
    }
}
