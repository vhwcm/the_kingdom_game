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
        "#%2i############"};

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
        "#%2i######"};

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
        "#%2i#####",
        "#        ",
        "#%2i     ",
        "#        ",
        "#        ",
        "#        ",
        "#%8s"};

    for (int i = 0; i < CARD_HEIGHT; i++)
    {
        if (i == 0)
            printw(lines[i], num);
        else if (i == 2)
            printw(lines[i], card.number);
        else if (i == CARD_HEIGHT - 1)
            printw(lines[i], card_suits[card.nipe].c_str());
        else
            printw(lines[i]);
        coord.down();
    }
}

void printFullCard(Coord &coord, Card card, int num)
{
    const char *lines[] = {
        "#%2i############",
        "#             #",
        "# %2i          #",
        "#             #",
        "#             #",
        "#             #",
        "#%8s     #"};

    for (int i = 0; i < CARD_HEIGHT; i++)
    {
        if (i == 0)
            printw(lines[i], num);
        else if (i == 2)
            printw(lines[i], card.number);
        else if (i == CARD_HEIGHT - 1)
            printw(lines[i], card_suits[card.nipe].c_str());
        else
            printw(lines[i]);
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
                board.addWarrior(card.number, player_time);
            }
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

void diamondsCard(bool player_time, Board &board, Player &p1, Player &p2)
{
    // Handle gold cards (diamonds)
    // 1. Add to diamond bank
    // 2. Check if can buy cards
    showMessage("Playing a Diamond card (Gold)");
    // Implementation needed
}

void clubsCard(bool player_time, Board &board, Player &p1, Player &p2)
{
    // Handle shield cards (clubs)
    // 1. Add shield to warrior
    // 2. Protect warrior from attacks
    showMessage("Playing a Club card (Shield)");
    // Implementation needed
}

void spadesCard(bool player_time, Board &board, Player &p1, Player &p2)
{
    // Handle attack cards (spades)
    // 1. Select target
    // 2. Calculate damage
    // 3. Apply effects
    showMessage("Playing a Spade card (Attack)");
    // Implementation needed
}

void jokerCard(bool player_time, Board &board, Player &p1, Player &p2)
{
    // Handle joker cards
    // 1. Steal random card from opponent
    showMessage("Playing a Joker card");
    // Implementation needed
}

bool playerTime(bool player_time, Board &board, Player &p1, Player &p2)
{
    const std::vector<std::string> moves = {"Select a Card or:", "'P'-Pass",
                                            "'Q'-quit"
                                            "'S'-Surrender"};
    if (player_time)
    {
        while (true)
        {
            printMoves(moves);
            int move = getint();
            if (!(move >= 0 && move < p1.hand.num()))
                showMessage("Select a valid Card of your Hand");
            else
            {
                switch (p1.hand.getCard(move).nipe)
                {
                case HEARTS: // Hearts
                    heartsCard(player_time, board, p1, p2, p1.hand.getCard(move));
                    break;
                case DIAMONDS: // Diamonds
                    diamondsCard(player_time, board, p1, p2);
                    break;
                case CLUBS: // Clubs
                    clubsCard(player_time, board, p1, p2);
                    break;
                case SPADES: // Spades
                    spadesCard(player_time, board, p1, p2);
                    break;
                case JOKER: // Joker
                    jokerCard(player_time, board, p1, p2);
                    break;
                default:
                    printw("move: %i", move);
                    showMessage("Invalid card type!");
                    break;
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
    board.draw(p1, p2, myCoord);
    bool player_time;
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
    while (!end_game)
    {
        end_game = playerTime(player_time, board, p1, p2);
        player_time = !player_time;
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