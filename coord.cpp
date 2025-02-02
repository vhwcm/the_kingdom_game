#include "the_kingdom.h"

Coord::Coord()
{
    yx = std::make_pair(0, 0);
}

void Coord::set(int y, int x)
{
    yx = std::make_pair(y, x);
    move(yx.first, yx.second);
}

void Coord::set()
{
    set(yx.first, yx.second);
}

void Coord::down(int y)
{
    yx.first += y;
    move(yx.first, yx.second);
}

void Coord::top(int y)
{
    yx.first -= y;
    move(yx.first, yx.second);
}

void Coord::left(int x)
{
    yx.second -= x;
    move(yx.first, yx.second);
}

void Coord::right(int x)
{
    yx.second += x;
    move(yx.first, yx.second);
}

std::pair<int, int> Coord::show()
{
    return yx;
}
