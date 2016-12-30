#include "simple_game.hpp"

#include <iostream>

simple_game::simple_game(sigma::context* ctx)
    : sigma::game(ctx)
{
    load("../../sigma-engine/simple_game/resources/simple.scn");
}

void simple_game::update(std::chrono::duration<float> dt)
{
    std::cout << " this is a test " << std::endl;
}

SIGMA_EXPORT_GAME_CLASSES(simple_game)
