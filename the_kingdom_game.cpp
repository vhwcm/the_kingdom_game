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