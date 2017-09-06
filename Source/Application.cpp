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
,   m_cells     (dataSize())
{
    std::mt19937 rng (std::time(nullptr));
    cellForEach([&](unsigned x, unsigned y)
    {
        std::uniform_int_distribution<int> dist(0, 1);
        auto& cell = m_cells[getCellIndex(x, y)];
        cell = (Cell)dist(rng);
    });

    m_view.setCenter(m_window.getSize().x / 2,
                   m_window.getSize().y / 2);
    m_window.setView(m_view);
}

void Application::run()
{
    unsigned generations = 0;
    sf::Clock clock;
    while (m_window.isOpen())
    {
        m_window.clear();

        if (m_running)
        {
            generations += 1;
            updateWorld();

            if (generations >= CONFIG.maxGenerations) {
                m_running = false;
                auto time = 1. * clock.getElapsedTime().asMilliseconds();
                std::cout << "Time taken per generation: " <<  time / generations << " milliseconds\n";
                std::cout << "Time taken per cell: " <<  time / generations / CONFIG.simWidth / CONFIG.simHeight * 1'000'000 << " nanoseconds\n";
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
    std::vector<Cell> newCells(dataSize());
    cellForEach([&](unsigned x, unsigned y)
    {
        unsigned count = 0;
        for (int nX = -1; nX <= 1; nX++)    //check neighbours
        for (int nY = -1; nY <= 1; nY++)
        {
            int newX = nX + x;
            int newY = nY + y;

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
                if (count < 3 || count > 4)
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
    return (y + 1) * (CONFIG.simWidth + 2) + x + 1;
}

void Application::updateQuads()
{
    for (unsigned y = 0; y < CONFIG.visibleSimHeight; ++y)
        for (unsigned x = 0; x < CONFIG.visibleSimWidth; ++x)
            m_quadBoard.setQuadColour(x, y, getCellColour(m_cells[getCellIndex(x + m_xOffset, y + m_yOffset)]));
}

unsigned Application::dataSize() const
{
    return (CONFIG.simWidth + 2) * (CONFIG.simHeight + 2);
}
