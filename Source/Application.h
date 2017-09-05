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
        void mouseInput         ();
        unsigned getCellIndex   (unsigned x, unsigned y) const;

        template<typename F>
        void cellForEach(F f) const;

        const Config CONFIG;

        QuadBoard m_quadBoard;
        State m_state;

        sf::RenderWindow m_window;
        sf::View m_view;
        sf::Font m_font;
        sf::Text m_text;

        std::vector<Cell> m_cells;
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
