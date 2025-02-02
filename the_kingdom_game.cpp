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

int main()
{
    initscr();
    clear();
    noecho();
    Coord myCoord;

    switch (printOptions(myCoord))
    {
    case 1:
        botGame();
        break;
    case 2:
        onlineGame(myCoord);
        break;
    case 3:
        gameInstructions();
        break;
    case 4:
        quit();
        break;
    default:
        break;
    }

    getch();

    clear();
    endwin();
    return 0;
}