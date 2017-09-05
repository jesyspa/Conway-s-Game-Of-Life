#include "Application.h"

#include <iostream>
#include <random>
#include <ctime>

namespace
{
    sf::Color deadColour   = {100, 100, 100};
    sf::Color aliveColour  = sf::Color::Black;
}

sf::Color& getCellColour(Cell cell)
{
    return cell == Cell::Alive ?
                    aliveColour :
                    deadColour;
}

Application::Application(const Config& config)
:   CONFIG      (config)
,   m_quadBoard (config)
,   m_window    ({config.simWidth, config.simHeight}, "Conway's Game of Life")
,   m_view      ({0, 0}, {(float)config.simWidth, (float)config.simHeight})
,   m_cells     (config.simWidth * config.simHeight)
{
    std::mt19937 rng (std::time(nullptr));
    cellForEach([&](unsigned x, unsigned y)
    {
        std::uniform_int_distribution<int> dist(0, 1);
        auto& cell = m_cells[getCellIndex(x, y)];
        cell = (Cell)dist(rng);
        m_quadBoard.addQuad(x, y, getCellColour(cell));
    });

    m_view.setCenter(m_window.getSize().x / 2,
                   m_window.getSize().y / 2);
}

void Application::run()
{
    int generations = 0;
    sf::Clock perf;
    while (m_window.isOpen())
    {
        m_window.clear();
        m_window.setView(m_view);

        if (m_running)
        {
            generations += 1;
            updateWorld();
            updateWorld();

            if (generations > CONFIG.maxGenerations)
            {
                m_running = false;
                auto totalTime = perf.getElapsedTime();
                std::cout << "Done in " << totalTime.asMilliseconds() << "ms.\n";
            }
        }

        m_quadBoard.draw(m_window);
        m_window.display();

        handleEvents();
    }
}

void Application::updateWorld()
{
    std::vector<Cell> newCells(CONFIG.simWidth * CONFIG.simHeight);
    cellForEach([&](unsigned x, unsigned y)
    {
        unsigned count = 0;
        for (int nX = -1; nX <= 1; nX++)    //check neighbours
        for (int nY = -1; nY <= 1; nY++)
        {
            int newX = nX + x;
            int newY = nY + y;

            if (newX == -1 || newX == (int)CONFIG.simWidth ||
                newY == -1 || newY == (int)CONFIG.simHeight || //out of bounds
                (nX == 0 && nY == 0)) //Cell itself
            {
                continue;
            }
            auto cell = m_cells[getCellIndex(newX, newY)];
            if (cell == Cell::Alive)
                count++;
        }

        auto cell           = m_cells[getCellIndex(x, y)];
        auto& updateCell    = newCells[getCellIndex(x, y)];
        updateCell = cell;
        switch (cell)
        {
            case Cell::Alive:
                if (count < 2 || count > 3)
                {
                    updateCell = Cell::Dead;
                }
                break;

            case Cell::Dead:
                if (count == 3)
                {
                    updateCell = Cell::Alive;
                }
                break;
        }

        m_quadBoard.setQuadColour(x, y, getCellColour(updateCell));
    });
    m_cells = std::move(newCells);
}


void Application::handleEvents()
{
    sf::Event e;

    while (m_window.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
        {
            m_window.close();
        }
    }
}

unsigned Application::getCellIndex(unsigned x, unsigned y) const
{
    return y * CONFIG.simWidth + x;
}
