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

// Implementação da classe Coord
Coord::Coord() {
    yx = std::make_pair(0, 0);
}

void Coord::set(int y, int x) {
    yx = std::make_pair(y, x);
    move(yx.first, yx.second);
}

void Coord::set() {
    set(yx.first, yx.second);
}

void Coord::down() {
    yx.first += 1;
    move(yx.first, yx.second);
}

void Coord::top() {
    yx.first -= 1;
    move(yx.first, yx.second);
}

void Coord::left() {
    yx.second -= 1;
    move(yx.first, yx.second);
}

void Coord::right() {
    yx.second += 1;
    move(yx.first, yx.second);
}

std::pair<int, int> Coord::show() {
    return yx;
}

// Implementação da classe Deck
Deck::Deck() {
    qnt = NUM_CARDS;
    for (short i = 0; i < NUM_NIPES; i++) {
        for (short j = 0; j < NUM_TYPES; j++) {
            cards.push_back(std::make_pair(i, j));
        }
        cards.push_back(std::make_pair(4, 13));
        cards.push_back(std::make_pair(4, 13));
    }
}

void Deck::shuffDeck() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

std::pair<int, int> Deck::drawCard() {
    if (cards.empty()) {
        throw std::out_of_range("No more cards in the deck");
    }
    std::pair<int, int> top = cards.front();
    cards.pop_back();
    qnt--;
    return top;
}

// Implementação da classe Hand
void Hand::setHand(Deck& deck) {
    qnt = 0;
    for (int i = 0; i < NUM_START_HAND; i++)
        drawFromDeck(deck);
}

void Hand::showCards() const {
    std::cout << "Your Hand:" << std::endl;
    for (const auto& card : cards) {
        std::cout << card_values[card.first] << " of " << card_suits[card.second] << std::endl;
    }
}

void Hand::drawFromDeck(Deck& deck) {
    try {
        std::pair<int, int> card = deck.drawCard();
        cards.push_back(card);
        qnt++;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
    }
}

// Implementação das funções
void serverMode(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Criar socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Erro ao criar o socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configurar o socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Erro ao configurar o socket" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Associar o socket ao endereço e porta
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Erro ao fazer bind" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Colocar o socket em modo de escuta
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Erro ao colocar em modo de escuta" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Esperando por conexões..." << std::endl;

    // Aceitar conexões de clientes
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Erro ao aceitar conexão" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Cliente conectado!" << std::endl;
    handleConnection(new_socket);

    // Fechar o socket
    close(new_socket);
}

void clientMode(const std::string& server_ip, int port) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Criar socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Erro ao criar o socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Converter endereço IP de texto para binário
    if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Endereço inválido" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Conectar ao servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Erro ao conectar" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Conectado ao servidor!" << std::endl;
    handleConnection(sock);

    // Fechar o socket
    close(sock);
}

void handleConnection(int socket) {
    char buffer[1024] = {0};
    std::string message;

    std::thread receiveThread([&]() {
        while (true) {
            int valread = read(socket, buffer, 1024);
            if (valread > 0) {
                std::cout << "Mensagem recebida: " << buffer << std::endl;
                memset(buffer, 0, sizeof(buffer));
            }
        }
    });

    while (true) {
        std::getline(std::cin, message);
        send(socket, message.c_str(), message.length(), 0);
    }

    receiveThread.join();
}

void printTitle(Coord& myCoord) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    if (cols > 135 && rows > 10) {
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

void printOptions(Coord& myCoord) {
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
    while (true){
        choice = getint();
        if(choice == 3){
            gameInstructions();
        }
        else if(choice < 1 || choice > 4){
            printw("Press 1,2,3 or 4!!!");
            myCoord.down();
        }else 
            break;
    }
    
    switch (choice) {
    case 1:
        botGame(myCoord);
        break;
    case 2:
        onlineGame(myCoord);
        break;
    case 4:
        quit();
        break;
    default:
        break;
    }
}

int getint(){
    return (getch() - '0');
}

void botGame(Coord& myCoord) {
    // Implementação do jogo contra bot
}

void onlineGame(Coord& myCoord) {
    // Implementação do jogo online
}
void gameInstructions() {
    int height, width;
    getmaxyx(stdscr, height, width);
    int start_y = 0;
    int start_x = 0;
    WINDOW* instructionWin = newwin(height, width, start_y, start_x);
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
    for (int i = 2; i < 20; i++) {
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
    while (exit != 'e' && exit != 'E') {
        exit = getch();
    }

    // Deletar a janela
    werase(instructionWin);
    wrefresh(instructionWin);
    delwin(instructionWin);

    touchwin(stdscr);
    refresh();
}

void printQuit(Coord& myCoord) {
    int row, col;
    getmaxyx(stdscr, row, col); // Obtém o número de linhas e colunas da janela
    mvprintw(row - 1, 0, "Pressione 'q' para sair.");
    myCoord.set();
    refresh();
}

void quit() {
    clear();
    endwin();
    exit(0);
}