#include "Application.h"

#include <random>
#include <ctime>
#include <iostream>

namespace
{
    sf::Color deadColour   = {100, 100, 100};
    sf::Color aliveColour  = sf::Color::Black;

    sf::Color& getCellColour(Cell cell)
    {
        return cell == Cell::Alive ?
                        aliveColour :
                        deadColour;
    }
}

Application::Application(const Config& config)
:   CONFIG      (config)
,   m_xOffset   ((config.simWidth  - config.visibleSimWidth) / 2)
,   m_yOffset   ((config.simHeight - config.visibleSimHeight) / 2)
,   m_quadBoard (config)
,   m_window    ({config.windowWidth, config.windowHeight}, "Conway's Game of Life")
,   m_view      ({0, 0}, {(float)config.windowWidth, (float)config.windowHeight})
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
    m_window.setView(m_view);
}

void Application::run()
{
    bool changed = false;
    int generations = 0;
    sf::Clock clock;
    while (m_window.isOpen())
    {
        m_window.clear();

        if (m_running)
        {
            generations += 1;
            updateWorld();

            if (generations > CONFIG.maxGenerations) {
                m_running = false;
                std::cout << "Time taken: " << clock.getElapsedTime().asMilliseconds() << '\n';
            }
        }

        updateQuads();
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

void Application::updateQuads()
{
    for (unsigned y = 0; y < CONFIG.visibleSimHeight; ++y)
        for (unsigned x = 0; x < CONFIG.visibleSimWidth; ++x)
            m_quadBoard.setQuadColour(x, y, getCellColour(m_cells[getCellIndex(x + m_xOffset, y + m_yOffset)]));
}
