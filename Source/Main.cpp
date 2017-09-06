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

int main(int argc, char**)
{
    Config config;

    if (argc > 1)
    {
        std::cout << "Enter simulation width: ";
        std::cin >> config.simWidth;

        std::cout << "Enter simulation height: ";
        std::cin >> config.simHeight;

        std::cout << "Enter window width: ";
        std::cin >> config.windowWidth;

        std::cout << "Enter window height: ";
        std::cin >> config.windowHeight;

        std::cout << "Enter cell size: ";
        std::cin >> config.quadSize;

        std::cout << "Enter number of generations to run for: ";
        std::cin >> config.maxGenerations;

        //Make it so that the cells fit in the window
        config.windowWidth -= config.windowWidth   % config.quadSize;
        config.windowHeight -= config.windowHeight % config.quadSize;

        config.windowWidth  =  std::min(config.simWidth * config.quadSize, config.windowWidth);
        config.windowHeight =  std::min(config.simHeight * config.quadSize, config.windowHeight);
    }
    else
    {
        config.quadSize       = 4;
        config.simWidth       = 4096;
        config.simHeight      = 4096;
        config.windowWidth    = 1280;
        config.windowHeight   = 720;
        config.maxGenerations = 200;
    }

    config.visibleSimWidth = config.windowWidth / config.quadSize;
    config.visibleSimHeight = config.windowHeight / config.quadSize;

    Application app(config);
    app.run();
}
