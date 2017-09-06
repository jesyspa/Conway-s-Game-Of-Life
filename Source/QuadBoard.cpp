#include "QuadBoard.h"

QuadBoard::QuadBoard(const Config& config)
:   CONFIG  (config)
{
    m_pixels.reserve(config.simWidth * config.simHeight * 4);
}

void QuadBoard::addQuad(unsigned x, unsigned y, sf::Color& colour)
{
    sf::Vertex topLeft;
    sf::Vertex topRight;
    sf::Vertex bottomLeft;
    sf::Vertex bottomRight;

    float pixelX = x * CONFIG.quadSize;
    float pixelY = y * CONFIG.quadSize;

    topLeft     .position = {pixelX,                    pixelY};
    topRight    .position = {pixelX + CONFIG.quadSize,  pixelY};
    bottomLeft  .position = {pixelX,                    pixelY + CONFIG.quadSize};
    bottomRight .position = {pixelX + CONFIG.quadSize,  pixelY + CONFIG.quadSize};

    topLeft     .color = colour;
    topRight    .color = colour;
    bottomLeft  .color = colour;
    bottomRight .color = colour;

    m_pixels.push_back(topLeft);
    m_pixels.push_back(bottomLeft);
    m_pixels.push_back(bottomRight);
    m_pixels.push_back(topRight);
}

void QuadBoard::setQuadColour(unsigned x, unsigned y, sf::Color& colour)
{
    auto index = getQuadIndex(x, y);

    for (int i = 0; i < 4; i++)
    {
        m_pixels[index + i].color = colour;
    }
}

void QuadBoard::draw(sf::RenderWindow& window)
{
    window.draw(m_pixels.data(), m_pixels.size(), sf::Quads);
}

unsigned QuadBoard::getQuadIndex(unsigned x, unsigned y) const
{
    return (y * CONFIG.simWidth + x) * 4;
}

