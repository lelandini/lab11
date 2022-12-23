#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>
#include "Game.h"

int main()
{
    setlocale(LC_ALL, "RU");
    srand(time(NULL));
    Game* new_game = new Game();
    new_game->gameSimulation();
    delete new_game;
    return 0;
}