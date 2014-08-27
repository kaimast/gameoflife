#include "Game.h"

int main()
{
    Game game;

    if(!game.init())
        return -1;

    while(game.isOk())
        game.update();

    return 0;
}

