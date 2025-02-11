#include "the_kingdom.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <ncurses.h>
#include <unistd.h>
#include <random>

void showMessage(const char *message)
{
    int height = 10;
    int width = strlen(message) + 4;    // Largura suficiente para a mensagem e borda
    int start_y = (LINES - height) / 2; // Centralizar verticalmente
    int start_x = (COLS - width) / 2;   // Centralizar horizontalmente

    WINDOW *messageWin = newwin(height, width, start_y, start_x);
    box(messageWin, 0, 0);
    mvwprintw(messageWin, 1, 2, "%s", message);
    wrefresh(messageWin);

    sleep(2); // Esperar por 2 segundos

    werase(messageWin);
    wrefresh(messageWin);
    delwin(messageWin);

    touchwin(stdscr);
    refresh();
}

void printEnemyFullCardBack(Coord &coord, int num)
{
    const char *lines[] = {
        "#             #",
        "#             #",
        "#             #",
        "#             #",
        "#             #",
        "#             #",
        "#%-2i############"};

    for (int i = 0; i < CARD_HEIGHT; i++)
    {
        if (i == CARD_HEIGHT - 1)
            printw(lines[i], num);
        else
            printw("%s", lines[i]); // Usando literal "%s"
        coord.down();
    }
}

void printEnemyHalfCardBack(Coord &coord, int num)
{
    const char *lines[] = {
        "#        ",
        "#        ",
        "#        ",
        "#        ",
        "#        ",
        "#        ",
        "#%-2i######"};

    for (int i = 0; i < CARD_HEIGHT; i++)
    {
        if (i == CARD_HEIGHT - 1)
            printw(lines[i], num);
        else
            printw("%s", lines[i]); // Usando literal "%s"
        coord.down();
    }
}

void printHalfCard(Coord &coord, Card card, int num)
{
    const char *lines[] = {
        "#%-2i#####",
        "#        ",
        "#        ",
        "#        ",
        "#        ",
        "#%-2i      ",
        "#%-8s"};

    for (int i = 0; i < CARD_HEIGHT; i++)
    {
        if (i == 0)
            printw(lines[i], num);
        else if (i == CARD_HEIGHT - 2)
            printw(lines[i], card.number);
        else if (i == CARD_HEIGHT - 1)
            printw(lines[i], card_suits[card.nipe].c_str());
        else
            printw("%s", lines[i]);
        coord.down();
    }
}

void printFullCard(Coord &coord, Card card, int num)
{
    const char *lines[] = {
        "#%-2i############",
        "#             #",
        "#             #",
        "#             #",
        "#             #",
        "#%-2i           #",
        "#%-8s     #"};

    for (int i = 0; i < CARD_HEIGHT; i++)
    {
        if (i == 0)
            printw(lines[i], num);
        else if (i == CARD_HEIGHT - 2)
            printw(lines[i], card.number);
        else if (i == CARD_HEIGHT - 1)
            printw(lines[i], card_suits[card.nipe].c_str());
        else
            printw("%s", lines[i]);
        coord.down();
    }
}

void printFullHalfCard(Coord &coord, Card card, int num)
{
    const char *lines[] = {
        "#%-2i#####",  // Line 0: shows the card number indicator (num)
        "#        ",   // Line 1
        "#        ",   // Line 2: shows the card's face value (card.number)
        "#        ",   // Line 3
        "#        ",   // Line 4 (extra line)
        "#        ",   // Line 5 (extra line)
        "#        ",   // Line 6 (extra line)
        "#%-2i      ", // Line 8 (extra line)
        "#%-8s",       // Line 9: shows the card suit
        "#########"    // Line 7 (extra line)
    };

    for (int i = 0; i < FULL_CARD_HEIGTH; i++)
    {
        if (i == 0)
            printw(lines[i], num);
        else if (i == FULL_CARD_HEIGTH - 3)
            printw(lines[i], card.number);
        else if (i == FULL_CARD_HEIGTH - 2)
            printw(lines[i], card_suits[card.nipe].c_str());
        else
            printw("%s", lines[i]);
        coord.down();
    }
}

void printFullFullCard(Coord &coord, Card card, int num)
{
    const char *lines[] = {
        "#%2i############",  // Line 0: prints "num" in a field of 2, then border characters to fill width 15
        "#             #",   // Line 1: blank line with left and right borders
        "#             #",   // Line 2: prints card.number (face value) inside borders
        "#             #",   // Line 3
        "#             #",   // Line 4
        "#             #",   // Line 5
        "#             #",   // Line 6
        "#%-2i           #", // Line 8
        "#%-13s#",           // Line 9: prints the card suit right-aligned in a field of 13 between borders
        "###############"    // Line 7
    };

    // Use FULL_CARD_HEIGTH (10) and CARD_WIDTH (15)
    for (int i = 0; i < FULL_CARD_HEIGTH; i++)
    {
        if (i == 0)
            printw(lines[i], num);
        else if (i == FULL_CARD_HEIGTH - 3)
            printw(lines[i], card.number);
        else if (i == FULL_CARD_HEIGTH - 2)
            printw(lines[i], card_suits[card.nipe].c_str());
        else
            printw("%s", lines[i]);
        coord.down();
    }
}

int getint()
{
    return (getch() - '0');
}

void printTitle(Coord &myCoord)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    if (cols > 135 && rows > 10)
    {
        printw(" # #####  ##  ##  #######           ### ###   ######  ##   ##   #####   #####     #####   ##   ##            #####     ###    ##   ##  ####### ");
        myCoord.down();
        printw("## ## ##  ##  ##   ##   #            ## ##      ##    ###  ##  ##   ##   ## ##   ### ###  ### ###           ##   ##   ## ##   ### ###   ##   #  ");
        myCoord.down();
        printw("   ##     ##  ##   ##                ####       ##    #### ##  ##        ##  ##  ##   ##  #######           ##       ##   ##  #######   ##      ");
        myCoord.down();
        printw("   ##     ######   ####              ###        ##    #######  ## ####   ##  ##  ##   ##  ## # ##           ## ####  ##   ##  ## # ##   ####    ");
        myCoord.down();
        printw("   ##     ##  ##   ##                ####       ##    ## ####  ##   ##   ##  ##  ##   ##  ##   ##           ##   ##  #######  ##   ##   ##      ");
        myCoord.down();
        printw("   ##     ##  ##   ##   #            ## ##      ##    ##  ###  ##   ##   ## ##   ### ###  ##   ##           ##   ##  ##   ##  ##   ##   ##   #  ");
        myCoord.down();
        printw(" ####    ##  ##  #######           ### ###   ######  ##   ##   #####   #####     #####   ### ###            #####   ##   ##  ### ###  ####### ");
        myCoord.down();
        myCoord.down();
    }
    else
    {
        printw("The kingdom game");
        myCoord.down();
        myCoord.down();
    }
}

int printOptions(Coord &myCoord)
{
    printTitle(myCoord);
    printw("Escolha uma opção: ");
    myCoord.down();
    printw("1-Jogar Contra Bot");
    myCoord.down();
    printw("2-Jogar Online na mesma rede");
    myCoord.down();
    printw("3-Ver regras do jogo");
    myCoord.down();
    printw("4-Sair");
    myCoord.down();
    refresh();

    int choice;
    while (true)
    {
        choice = getint();
        if (choice == 3)
        {
            gameInstructions();
        }
        else if (choice < 1 || choice > 4)
        {
            printw("Press 1,2,3 or 4!!!");
            myCoord.down();
        }
        else
            break;
    }

    return choice;
}

void printMoves(std::vector<std::string> moves)
{
    Coord coord;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    coord.set(max_y - 1, 0);
    clrtoeol();
    for (auto move : moves)
    {
        printw("%-20s ", move.c_str());
    }
    refresh();
}
// Function implementations for each card type

int heartsCard(bool player_time, Board &board, Player &p1, Player &p2, Card heartsCard)
{
    // Adicionada validação do nipe
    if (heartsCard.nipe != HEARTS)
    {
        showMessage("Carta inválida para Hearts!");
        return -1;
    }
    std::vector<std::string> moves = {"|Select other card|", "|'M'-move the warrior(s) to Board|",
                                      "|'D'-drop the card(s)|",
                                      "|'P'-pass", "Combo Cards:"};

    std::vector<Card> combo;
    combo.push_back(heartsCard);
    for (auto card : combo)
    {
        char buffer[15];
        snprintf(buffer, sizeof(buffer), "%i - %s", card.number, card_suits[card.nipe].c_str());
        moves.push_back(buffer);
    }

    while (true)
    {
        printMoves(moves);
        char move = getch();
        if (move == 'D' || move == 'd')
        {
            return 1;
        }
        else if (move == 'P' || move == 'p')
        {
            return 2;
        }
        else if (move == 'M' || move == 'm')
        {
            for (auto card : combo)
            {
                board.addWarrior(card, player_time);
                p1.hand.removeCard(card);
            }

            return 0;
        }
        else
        {
            int int_move = move - '0';
            if (int_move < 0)
            {
                showMessage("Invalid Move");
            }
            else if (int_move >= p1.hand.num())
            {
                showMessage("Invalid Move");
            }
            else if (p1.hand.getCard(int_move).nipe != HEARTS)
            {
                showMessage("Card is not an Hearts");
            }
            else
            {
                auto it = std::find(combo.begin(), combo.end(), p1.hand.getCard(int_move));
                if (it == combo.end())
                {
                    combo.push_back(p1.hand.getCard(int_move));
                    char buffer[50];
                    snprintf(buffer, sizeof(buffer), "%i - %s", p1.hand.getCard(int_move).number, card_suits[p1.hand.getCard(int_move).nipe].c_str());
                    moves.push_back(buffer);
                }
                else
                    showMessage("This card already was chosed");
            }
        }
    }
}

void diamondsCard(bool player_time, Board &board, Player &p1, Player &p2, Card diamondCard)
{
    // Adicionada validação do nipe
    if (diamondCard.nipe != DIAMONDS)
    {
        showMessage("Carta inválida para Diamonds!");
        return;
    }
    std::vector<std::string> moves = {"|Select other diamond|", "|'B'-Buy cards|",
                                      "|'D'-drop the card(s)|", "|'P'-pass|",
                                      "Combo Cards:"};

    std::vector<Card> combo;
    combo.push_back(diamondCard);
    int totalGold = diamondCard.number;

    while (true)
    {
        // Update available moves display
        moves.clear();
        moves = {"|Select other diamond|",
                 "|'B'-Buy cards (" + std::to_string(totalGold / 5) + " available)|",
                 "|'D'-drop the card(s)|",
                 "|'P'-pass|",
                 "Combo Cards:"};

        for (auto card : combo)
        {
            char buffer[15];
            snprintf(buffer, sizeof(buffer), "%i - %s", card.number, card_suits[card.nipe].c_str());
            moves.push_back(buffer);
        }
        printMoves(moves);

        char move = getch();
        if (move == 'D' || move == 'd')
        {
            return;
        }
        else if (move == 'P' || move == 'p')
        {
            return;
        }
        else if (move == 'B' || move == 'b')
        {
            int cardsCanBuy = totalGold / 5;
            if (cardsCanBuy > 0)
            {
                // Draw cards based on gold value
                p1.drawnFromDeck(cardsCanBuy);
                return;
            }
            else
            {
                showMessage("Not enough gold to buy cards!");
            }
        }
        else
        {
            int int_move = move - '0';
            if (int_move >= 0 && int_move < p1.hand.num())
            {
                Card selectedCard = p1.hand.getCard(int_move);
                if (selectedCard.nipe == DIAMONDS)
                {
                    auto it = std::find(combo.begin(), combo.end(), selectedCard);
                    if (it == combo.end())
                    {
                        combo.push_back(selectedCard);
                        totalGold += selectedCard.number;
                    }
                    else
                    {
                        showMessage("This card already was chosen");
                    }
                }
                else
                {
                    showMessage("Card is not a Diamond");
                }
            }
            else
            {
                showMessage("Invalid Move");
            }
        }
    }
}

void clubsCard(bool player_time, Board &board, Player &p1, Player &p2, Card clubCard)
{
    // Adicionada validação do nipe
    if (clubCard.nipe != CLUBS)
    {
        showMessage("Carta inválida para Clubs!");
        return;
    }
    std::vector<std::string> moves = {"|Select warrior to shield|", "Select diamond nank",
                                      "|'P'-pass|"};

    while (true)
    {
        printMoves(moves);
        char move = getch();
        if (move == 'P' || move == 'p')
        {
            return;
        }
        else
        {
            int warrior_pos = move - '0';
            // Add shield to selected warrior
            int result = board.addShield(warrior_pos, clubCard, player_time);

            switch (result)
            {
            case 0: // Success
                p1.hand.removeCard(clubCard);
                return;
            case 1: // Invalid position
                showMessage("Invalid warrior position!");
                break;
            case 2: // Already has shield
                showMessage("Warrior already has a shield!");
                break;
            default:
                showMessage("Invalid move!");
                break;
            }
        }
    }
}

void spadesCard(bool player_time, Board &board, Player &p1, Player &p2, Card spadeCard)
{
    // Adicionada validação do nipe
    if (spadeCard.nipe != SPADES)
    {
        showMessage("Carta inválida para Spades!");
        return;
    }
    std::vector<std::string> moves = {"|Select target to attack|",
                                      "|Select additional spades|",
                                      "|'A'-Attack|",
                                      "|'D'-drop the card(s)|",
                                      "|'P'-pass|",
                                      "Attack Cards:"};

    std::vector<Card> combo;
    combo.push_back(spadeCard);
    int totalDamage = spadeCard.number;
    int target = -1;

    while (true)
    {
        // Update moves with current attack power
        moves.clear();
        moves = {"|Select target to attack|",
                 "|Select additional spades|",
                 "|'A'-Attack (Power: " + std::to_string(totalDamage) + ")|",
                 "|'D'-drop the card(s)|",
                 "|'P'-pass|",
                 "Attack Cards:"};

        for (auto card : combo)
        {
            char buffer[15];
            snprintf(buffer, sizeof(buffer), "%i - %s", card.number, card_suits[card.nipe].c_str());
            moves.push_back(buffer);
        }
        printMoves(moves);

        char move = getch();
        if (move == 'D' || move == 'd')
        {
            return;
        }
        else if (move == 'P' || move == 'p')
        {
            return;
        }
        else if (move == 'A' || move == 'a')
        {
            if (target >= 0)
            {
                int result = board.atackWarrior(target, totalDamage, player_time);
                switch (result)
                {
                case 1: // Shield broken
                    showMessage("Shield broken!");
                    return;
                case 2: // Warrior killed
                    showMessage("Warrior killed!");
                    p1.drawnFromDeck(1); // Bonus card for kill
                    return;
                case -1:
                    showMessage("Invalid target!");
                    break;
                case -2:
                    showMessage("Not enough damage to break shield!");
                    break;
                case -3:
                    showMessage("Not enough damage to kill warrior!");
                    break;
                }
            }
            else
            {
                showMessage("Select a target first!");
            }
        }
        else
        {
            int int_move = move - '0';
            if (target < 0)
            {
                target = int_move;
            }
            else if (int_move >= 0 && int_move < p1.hand.num())
            {
                Card selectedCard = p1.hand.getCard(int_move);
                if (selectedCard.nipe == SPADES)
                {
                    auto it = std::find(combo.begin(), combo.end(), selectedCard);
                    if (it == combo.end())
                    {
                        combo.push_back(selectedCard);
                        totalDamage += selectedCard.number;
                    }
                    else
                    {
                        showMessage("This card already was chosen");
                    }
                }
                else
                {
                    showMessage("Card is not a Spade");
                }
            }
            else
            {
                showMessage("Invalid Move");
            }
        }
    }
}

void jokerCard(bool player_time, Board &board, Player &p1, Player &p2, Card jokerCard)
{
    // Adicionada validação do nipe
    if (jokerCard.nipe != JOKER)
    {
        showMessage("Carta inválida para Joker!");
        return;
    }
    // Joker steals a random card from opponent
    if (p2.hand.num() > 0)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, p2.hand.num() - 1);
        int stolen_card_pos = distrib(gen);

        Card stolenCard = p2.hand.getCard(stolen_card_pos);
        p1.hand.addCard(stolenCard);

        char buffer[50];
        snprintf(buffer, sizeof(buffer), "Stole a %s of %s!",
                 card_values[stolenCard.number - 1].c_str(),
                 card_suits[stolenCard.nipe].c_str());
        showMessage(buffer);
    }
    else
    {
        showMessage("Opponent has no cards to steal!");
    }
}

bool playerTime(int player_time, Board &board, Player &p1, Player &p2)
{
    const std::vector<std::string> moves = {"Select a Card or:", "'P'-Pass",
                                            "'Q'-quit"
                                            "'S'-Surrender"};
    if (player_time)
    {
        while (true)
        {
            board.draw(p1, p2);
            printMoves(moves);
            int move = getch();
            if (move == 'Q' || move == 'q')
            {
                quit();
            }
            else
            {
                int int_move = move - '0';
                if (!(int_move >= 0 && int_move < p1.hand.num()))
                    showMessage("Select a valid Card of your Hand");
                else
                {
                    switch (p1.hand.getCard(int_move).nipe)
                    {
                    case HEARTS: // Hearts
                        heartsCard(player_time, board, p1, p2, p1.hand.getCard(int_move));
                        break;
                    case DIAMONDS: // Diamonds
                        diamondsCard(player_time, board, p1, p2, p1.hand.getCard(int_move));
                        break;
                    case CLUBS: // Clubs
                        clubsCard(player_time, board, p1, p2, p1.hand.getCard(int_move));
                        break;
                    case SPADES: // Spades
                        spadesCard(player_time, board, p1, p2, p1.hand.getCard(int_move));
                        break;
                    case JOKER: // Joker
                        jokerCard(player_time, board, p1, p2, p1.hand.getCard(int_move));
                        break;
                    default:
                        printw("move: %i", move);
                        showMessage("Invalid card type!");
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // Bot logic will go here
        return false;
    }
    return false;
}

void loopBotGame(Board &board, Player &p1, Player &p2)
{
    std::random_device rd;  // Gerador de números aleatórios não determinístico
    std::mt19937 gen(rd()); //  // Mersenne Twister, um gerador de números pseudo-aleatórios
    std::uniform_int_distribution<> distrib(1, 2);
    Coord myCoord;
    myCoord.set(1, 1);
    board.draw(p1, p2);
    int player_time;
    bool end_game = 0;
    if (distrib(gen) == 1)
    {
        showMessage("Player 1 starts!!!");
        player_time = 0;
    }
    else
    {
        showMessage("Player 2 Starts!!!");
        player_time = 1;
    }
    player_time = 1;
    while (!end_game)
    {
        end_game = playerTime(player_time, board, p1, p2);
        // player_time = !player_time;
    }
}

void botGame()
{
    // Implementação do jogo contra bot
    werase(stdscr); // limpa a tela
    Board board;
    Player p1(1);
    Player p2(2);
    p1.deck.shuffDeck();
    p2.deck.shuffDeck();
    p2.drawnFromDeck(3);
    p1.drawnFromDeck(3);
    refresh();
    loopBotGame(board, p1, p2);
}

void onlineGame(Coord &myCoord)
{
    // Implementação do jogo online
}
void gameInstructions()
{
    int height, width;
    getmaxyx(stdscr, height, width);
    int start_y = 0;
    int start_x = 0;
    WINDOW *instructionWin = newwin(height, width, start_y, start_x);
    box(instructionWin, 0, 0);
    wrefresh(instructionWin);

    // Adicionar instruções na janela
    mvwprintw(instructionWin, 1, 1, "Game Instructions:");
    mvwprintw(instructionWin, 3, 1, "1- Objective: get the king, queen,");
    mvwprintw(instructionWin, 5, 1, "   and jack of the same suit");
    mvwprintw(instructionWin, 7, 1, "2- Each player has a deck and a board");
    mvwprintw(instructionWin, 9, 1, "3- The game is turn-based, first player");
    mvwprintw(instructionWin, 11, 1, "   is chosen randomly.");
    mvwprintw(instructionWin, 13, 1, "4- At the start of each turn, draw a card");
    mvwprintw(instructionWin, 15, 1, "   from your deck");
    mvwprintw(instructionWin, 17, 1, "5- Players can place heart cards on the board");
    mvwprintw(instructionWin, 19, 1, "6- If you have more lives (sum of heart cards)");
    mvwprintw(instructionWin, 21, 1, "   than your opponent, draw an extra card");
    mvwprintw(instructionWin, 23, 1, "7- By killing a warrior, draw a card");
    mvwprintw(instructionWin, 25, 1, "   immediately");
    mvwprintw(instructionWin, 27, 1, "8- On your turn, use the ability of each card");
    mvwprintw(instructionWin, 29, 1, "9- Kingdom cards have additional abilities");
    mvwprintw(instructionWin, 31, 1, "10- Cards can be combined for a higher value");
    mvwprintw(instructionWin, 33, 1, "11- Have fun!");
    mvwprintw(instructionWin, 35, 1, "Press 'e' to quit.");

    mvwprintw(instructionWin, 1, 50, "|");
    for (int i = 2; i < 20; i++)
    {
        mvwprintw(instructionWin, i, 50, "|");
    }
    mvwprintw(instructionWin, 2, 52, "Abilities by Suit");
    mvwprintw(instructionWin, 3, 52, "Diamonds:");
    mvwprintw(instructionWin, 4, 52, "1 - Diamond cards give you gold equal to their number.");
    mvwprintw(instructionWin, 5, 52, "2 - You can buy cards with gold, each card costs 5 gold.");
    mvwprintw(instructionWin, 6, 52, "3 - If you have a 3 and a 7 of diamonds, you can use them");
    mvwprintw(instructionWin, 7, 52, "    together to have 10 gold.");
    mvwprintw(instructionWin, 8, 52, "4 - Any excess value from a division by 5 will be discarded.");
    mvwprintw(instructionWin, 9, 52, "5 - Gold can be placed in the gold bank. If you accumulate");
    mvwprintw(instructionWin, 10, 52, "    20 gold, buy an additional card at the start of each turn.");
    mvwprintw(instructionWin, 11, 52, "Hearts:");
    mvwprintw(instructionWin, 12, 52, "1 - Heart cards represent warriors.");
    mvwprintw(instructionWin, 13, 52, "2 - These warriors can be placed on the board.");
    mvwprintw(instructionWin, 14, 52, "3 - The life of these warriors is defined by the number on the card.");
    mvwprintw(instructionWin, 15, 52, "Clubs:");
    mvwprintw(instructionWin, 16, 52, "1 - Club cards serve as shields for the warriors.");
    mvwprintw(instructionWin, 17, 52, "2 - Each warrior can only have one shield.");
    mvwprintw(instructionWin, 18, 52, "3 - A warrior can only be attacked after their shield is broken.");
    mvwprintw(instructionWin, 19, 52, "4 - Excess damage to the shield does not reduce the warrior's life.");
    mvwprintw(instructionWin, 20, 52, "5 - If you want, you can break a warrior's shield.");
    mvwprintw(instructionWin, 21, 52, "Spades:");
    mvwprintw(instructionWin, 22, 52, "1 - Spade cards are used to attack warriors and their shields.");
    mvwprintw(instructionWin, 23, 52, "2 - They deal damage equal to their number.");
    mvwprintw(instructionWin, 24, 52, "3 - If the sum of the spade cards' numbers is greater than the shield");
    mvwprintw(instructionWin, 25, 52, "    or the warrior's life, the shield is broken or the warrior is killed.");
    mvwprintw(instructionWin, 26, 52, "Special Cards:");
    mvwprintw(instructionWin, 27, 52, "** Special cards have an additional power besides their suit's ability.");
    mvwprintw(instructionWin, 28, 52, "Queen: buy three cards");
    mvwprintw(instructionWin, 29, 52, "King: kill a warrior");
    mvwprintw(instructionWin, 30, 52, "Jack: swap one of your chosen warriors with one of the opponent's");
    mvwprintw(instructionWin, 31, 52, "Joker: Steal a card from the opponent, chosen blindly");

    wrefresh(instructionWin);

    // Esperar por uma entrada do usuário antes de fechar a janela
    char exit = 'a';
    while (exit != 'e' && exit != 'E')
    {
        exit = getch();
    }

    // Deletar a janela
    werase(instructionWin);
    wrefresh(instructionWin);
    delwin(instructionWin);

    touchwin(stdscr);
    refresh();
}

void printQuit(Coord &myCoord)
{
    int row, col;
    getmaxyx(stdscr, row, col); // Obtém o número de linhas e colunas da janela
    mvprintw(row - 1, 0, "Pressione 'q' para sair.");
    myCoord.set();
    refresh();
}

void quit()
{
    clear();
    endwin();
    exit(0);
}