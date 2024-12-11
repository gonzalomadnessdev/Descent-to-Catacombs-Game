#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    std::srand(std::time(0));
    Game game("RPG Game");
    
    game.Run();

    return 0;
}

