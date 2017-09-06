#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include "QuadBoard.h"
#include "Config.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>

enum class Cell
{
    Dead,
    Alive
};

class Application
{
    public:
        Application(const Config& config);

        void run();

    private:
        void handleEvents       ();
        void updateWorld        ();
        void updateQuads        ();
        unsigned getCellIndex   (unsigned x, unsigned y) const;
        unsigned dataSize       () const;


        template<typename F>
        void cellForEach(F f) const;

        const Config CONFIG;
        unsigned m_xOffset, m_yOffset;

        QuadBoard m_quadBoard;
        bool m_running = true;

        sf::RenderWindow m_window;
        sf::View m_view;

        std::vector<Cell> m_cells;
        std::vector<Cell> m_newCells;

        Cell* jumpToStart(Cell*) const;
        Cell* nextRow(Cell*) const;
        Cell* up(Cell*) const;
        Cell* down(Cell*) const;
        Cell* left(Cell*) const;
        Cell* right(Cell*) const;
        unsigned neighbourCount(Cell*) const;
};

template<typename F>
void Application::cellForEach(F f) const
{
    for (unsigned y = 0; y < CONFIG.simHeight; y++)
    {
        for (unsigned x = 0; x < CONFIG.simWidth; x++)
        {
            f(x, y);
        }
    }
}

#endif // APPLICATION_H_INCLUDED
