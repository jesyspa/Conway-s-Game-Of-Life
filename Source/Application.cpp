#include "Application.h"

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
,   m_state     (config.initialState)
,   m_window    ({config.windowWidth, config.windowHeight}, "Conway's Game of Life")
,   m_view      ({0, 0}, {(float)config.windowWidth, (float)config.windowHeight})
,   m_cells     (config.simWidth * config.simHeight)
{
    m_font.loadFromFile         ("font/arial.ttf");
    m_text.setFont              (m_font);
    m_text.setFillColor         (sf::Color::White);
    m_text.setOutlineColor      (sf::Color::Black);
    m_text.setOutlineThickness  (3);
    m_text.setCharacterSize     (20);
    m_text.setPosition          (5, 5);
    m_text.setString            (std::string("Click cell to change it to alive/ dead.\n") +
                                 std::string("Press \"C\" to clear the cells\n") +
                                 std::string("Press \"T\" to toggle the grid\n") +
                                 std::string("Press \"Space\" when you are ready."));

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
    bool changed = false;
    int offset = 1;
    int generations = 0;
    int const max_generations = 1000;
    sf::Clock clock;
    while (m_window.isOpen())
    {
        m_window.clear();
        m_window.setView(m_view);

        switch (m_state)
        {
            case State::Creating:
                mouseInput();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    m_state = State::Sim;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                {
                    cellForEach([&](unsigned x, unsigned y)
                    {
                        m_cells[getCellIndex(x, y)] = Cell::Dead;
                        m_quadBoard.setQuadColour(x, y, deadColour);
                    });
                }
                break;

            case State::Sim:
                generations += 1;
                m_text.setString("Generation: " + std::to_string(generations));
                updateWorld();

                if (generations > max_generations)
                    m_state = State::Done;
                break;

            case State::Done:
                m_text.setString("Done!");
                break;
        }

        m_quadBoard.draw(m_window);
        m_window.setView(m_window.getDefaultView());
        m_window.draw(m_text);

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

void Application::mouseInput()
{
    static sf::Clock delay;
    if (delay.getElapsedTime().asSeconds() > 0.2)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            delay.restart();
            auto mousePosition = sf::Mouse::getPosition(m_window);
            auto adjustedMousePosition = m_window.mapCoordsToPixel({(float)mousePosition.x,
                                                                    (float)mousePosition.y});
            auto x = adjustedMousePosition.x;
            auto y = adjustedMousePosition.y;

            if (x < 0 || x > (int)m_window.getSize().x ||
                y < 0 || y > (int)m_window.getSize().y)
            {
                return;
            }

            //Convert mouse/ screen coordinates to cell coordinates
            int newX = x / CONFIG.quadSize;
            int newY = y / CONFIG.quadSize;

            //Switch cell type
            auto& cell = m_cells[getCellIndex(newX, newY)];
            cell =  cell == Cell::Alive ?
                        Cell::Dead :
                        Cell::Alive;

            //Set new colour
            m_quadBoard.setQuadColour(newX, newY, getCellColour(cell));
        }
    }
}
