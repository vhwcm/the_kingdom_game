// game.cpp
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <ncurses.h>
#include "the_kingdom.h"

void serverMode(int port);
void clientMode(const std::string& server_ip, int port);
void handleConnection(int socket);
void printTitle(Coord& myCoord);
void printOptions(Coord& myCoord);
void botGame(Coord& myCoord);
void onlineGame(Coord& myCoord);
void gameInstructions(Coord& myCoord);
void printQuit(Coord& myCoord);
void quit();

int main() {
    initscr();
    clear();
    noecho(); 
    Coord myCoord;
    printOptions(myCoord);
    
    getch();  

    clear();
    endwin();
    return 0;
}

void printTitle(Coord& myCoord){
    int rows,cols;
    getmaxyx(stdscr, rows, cols);
    if(cols > 135 && rows > 10){
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
    }else{
        printw("THE KINGDOM GAME!");
        myCoord.down();
        myCoord.down();
    }
}

void printOptions(Coord& myCoord){
    printTitle(myCoord);
    printw("1-Jogar Contra Bot");
    myCoord.down();
    printw("2-Jogar Online na mesma rede");
    myCoord.down();
    printw("3-Ver regras do jogo");
    printQuit(myCoord);
    refresh();
    int choice = -1;
    myCoord.down();
    myCoord.down();
    while (true) {
        choice = getch() - '0';
        if (choice == 1 || choice == 2) {
            break;
        } else if (choice == 3) {
            gameInstructions(myCoord);
        } else if (choice == 'q' - '0' || choice == 'Q' - '0') {
            quit();
        } else {
            printw("Pressione 1, 2 ou 3!!!");
            myCoord.down();
        }
    }

    switch (choice) {
    case 1:
        botGame(myCoord);
        break;
    case 2:
        onlineGame(myCoord);
        break;
    default:
        quit();
        break;
    }
    return;
}

void botGame(Coord& myCoord){
    return ;
}
void onlineGame(Coord& myCoord){

}

void gameInstructions(Coord& myCoord){
    int height = 70;
    int width = 100;
    int start_y = 0;
    int start_x = 5;
    WINDOW* instructionWin = newwin(height,width,start_y,start_x);
    box(instructionWin,0,0);
    // Adicionar instruções na janela
    mvwprintw(instructionWin, 1, 1, "Game Instructions:");
    mvwprintw(instructionWin, 2, 1, "1. Use arrow keys to move.");
    mvwprintw(instructionWin, 3, 1, "2. Press 'e' to quit.");
    mvwprintw(instructionWin, 4, 1, "3. Have fun!");

    // Atualizar a janela para exibir o conteúdo
    wrefresh(instructionWin);

    // Esperar por uma entrada do usuário antes de fechar a janela
    char exit = 'a';
    while (exit != 'e')
    {
        exit = getch();
    }

    // Deletar a janela
    werase(instructionWin);
    wrefresh(instructionWin);
    delwin(instructionWin);

    refresh();
}
void quit(){
    clear();
    endwin();
    exit(0);
}

void printQuit(Coord& myCoord){
    int row,col;
    getmaxyx(stdscr,row,col);
    mvprintw(row - 1, 0, "PRESS 'q' to exit.");
    myCoord.set();
}

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
    close(server_fd);
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