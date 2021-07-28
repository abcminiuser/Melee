#include "SFMLPlayerHudTileRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

SFMLPlayerHudTileRenderer::SFMLPlayerHudTileRenderer(PlayerEntity& entity, const sf::FloatRect& renderArea)
	: m_entity(entity)
    , m_renderArea(renderArea)
{
    m_font.loadFromFile("Assets/Fonts/8bitOperatorPlus8-Bold.ttf");
}

void SFMLPlayerHudTileRenderer::render(sf::RenderTarget& renderer)
{
	const float lifeNormalized = m_entity.health() / (float)m_entity.properties().maxHealth;
    const float energyNormalized = m_entity.energy() / (float)m_entity.properties().maxEnergy;

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

    {
        sf::RectangleShape r;
        r.setFillColor(sf::Color::Black);
        r.setPosition(m_renderArea.left + 30, m_renderArea.top + 40);
        r.setSize(sf::Vector2f{ 20, 100 });
        renderer.draw(r);

    }
    {
        sf::RectangleShape r;
        r.setFillColor(sf::Color(180, 0, 0));
        r.setPosition(m_renderArea.left + 30, m_renderArea.top + 40 + (1 - lifeNormalized) * 100);
        r.setSize(sf::Vector2f{ 20, lifeNormalized * 100 });
        renderer.draw(r);
    }
    {
        sf::Text text;
        text.setString("Health");
        text.setPosition(m_renderArea.left + 10, m_renderArea.top + 140);
        text.setFillColor(sf::Color::Black);
        text.setFont(m_font);
        text.setCharacterSize(16);
        renderer.draw(text);
    }

    {
        sf::RectangleShape r;
        r.setFillColor(sf::Color(0, 0, 80));
        r.setPosition(m_renderArea.left + 200, m_renderArea.top + 40);
        r.setSize(sf::Vector2f{ 20, 100 });
        renderer.draw(r);

    }
    {
        sf::RectangleShape r;
        r.setFillColor(sf::Color(50, 50, 255));
        r.setPosition(m_renderArea.left + 200, m_renderArea.top + 40 + (1 - energyNormalized) * 100);
        r.setSize(sf::Vector2f{ 20, energyNormalized * 100 });
        renderer.draw(r);
    }
    {
        sf::Text text;
        text.setString("Energy");
        text.setPosition(m_renderArea.left + 180, m_renderArea.top + 140);
        text.setFillColor(sf::Color::Black);
        text.setFont(m_font);
        text.setCharacterSize(16);
        renderer.draw(text);
    }
}
