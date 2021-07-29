#include "SFMLPlayerHudTileRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    static const auto kLifeBarInactiveFillColour = HSVColor(0, 100, 40);
    static const auto kLifeBarActiveFillColour = HSVColor(0, 100, 90);

    static const auto kEnergyBarInactiveFillColour = HSVColor(240, 100, 40);
    static const auto kEnergyBarActiveFillColour = HSVColor(240, 100, 90);
}

SFMLPlayerHudTileRenderer::SFMLPlayerHudTileRenderer(PlayerEntity& entity, const sf::FloatRect& renderArea)
	: m_entity(entity)
    , m_renderArea(renderArea)
{
    m_font.loadFromFile("Assets/Fonts/8bitOperatorPlus8-Bold.ttf");
}

void SFMLPlayerHudTileRenderer::render(sf::RenderTarget& renderer)
{
    sf::RectangleShape r;
    r.setFillColor(sf::Color(150, 150, 150));
    r.setPosition(m_renderArea.left, m_renderArea.top);
    r.setSize(sf::Vector2f{ m_renderArea.width, m_renderArea.height });
    renderer.draw(r);

    sf::Text text;
    text.setString("Player " + std::to_string(m_entity.index() + 1));
    text.setPosition(m_renderArea.left + 60, m_renderArea.top);
    text.setFillColor(sf::Color::Black);
    text.setFont(m_font);
    text.setCharacterSize(24);
    renderer.draw(text);

    const float healthNormalized = m_entity.health() / (float)m_entity.properties().maxHealth;
    drawVerticalBarGraph(renderer, sf::FloatRect{ m_renderArea.left + 30, m_renderArea.top + 40, 20, 100 }, kLifeBarInactiveFillColour, kLifeBarActiveFillColour, healthNormalized);

    {
        sf::Text text;
        text.setString("Health");
        text.setPosition(m_renderArea.left + 15, m_renderArea.top + 140);
        text.setFillColor(sf::Color::Black);
        text.setFont(m_font);
        text.setCharacterSize(16);
        renderer.draw(text);
    }

    const float energyNormalized = m_entity.energy() / (float)m_entity.properties().maxEnergy;
    drawVerticalBarGraph(renderer, sf::FloatRect{ m_renderArea.left + m_renderArea.width - 30 - 20, m_renderArea.top + 40, 20, 100 }, kEnergyBarInactiveFillColour, kEnergyBarActiveFillColour, energyNormalized);

    {
        sf::Text text;
        text.setString("Energy");
        text.setPosition(m_renderArea.left + 175, m_renderArea.top + 140);
        text.setFillColor(sf::Color::Black);
        text.setFont(m_font);
        text.setCharacterSize(16);
        renderer.draw(text);
    }
}

void SFMLPlayerHudTileRenderer::drawVerticalBarGraph(sf::RenderTarget& renderer, sf::FloatRect area, sf::Color inactiveColour, sf::Color activeColour, float valueNormalized)
{
    // Background fill
    {
        sf::RectangleShape bg;
        bg.setFillColor(inactiveColour);
        bg.setPosition(area.left, area.top);
        bg.setSize(sf::Vector2f{ area.width, area.height });
        renderer.draw(bg);
    }

    // Foreground fill
    {
        sf::RectangleShape fg;
        fg.setFillColor(activeColour);
        fg.setPosition(area.left, area.top + area.height * (1 - valueNormalized));
        fg.setSize(sf::Vector2f{ area.width, area.height * valueNormalized });
        renderer.draw(fg);
    }
}
