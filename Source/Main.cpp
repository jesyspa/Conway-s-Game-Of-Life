#include "Application.h"
#include "Config.h"

#include <iostream>

/**
  * Rules:
  *     The universe of the Game of Life is an infinite two-dimensional orthogonal grid of square cells,
        each of which is in one of two possible states, alive or dead, or "populated" or "unpopulated".
        Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or
        diagonally adjacent. At each step in time, the following transitions occur:
  *
  *  - Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
  *  - Any live cell with two or three live neighbours lives on to the next generation.
  *  - Any live cell with more than three live neighbours dies, as if by overpopulation.
  *  - Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
  */

int main(int argc, char** argv)
{
    Config config;

    if (argc > 1)
    {
        std::cout << "Enter window width: ";
        std::cin >> config.simWidth;

        std::cout << "Enter window height: ";
        std::cin >> config.simHeight;

        std::cout << "Enter generation count: ";
        std::cin >> config.maxGenerations;

        config.initialState = State::Creating;
    }
    else
    {
        config.simWidth       = 1280;
        config.simHeight      = 720;
        config.maxGenerations = 1000;
        config.initialState   = State::Sim;
    }

    Application app(config);
    app.run();
}
