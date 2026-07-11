#include "SFMLShipHudTileRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

#include <map>

using namespace Melee;

namespace
{
    static const auto kLifeBarInactiveFillColour = HSVColor(0, 100, 40);
    static const auto kLifeBarActiveFillColour = HSVColor(0, 100, 90);

    static const auto kEnergyBarInactiveFillColour = HSVColor(240, 100, 40);
    static const auto kEnergyBarActiveFillColour = HSVColor(240, 100, 90);

    static const std::map<ShipEntity::VisualType, std::string> kRaceNames
    {
        { ShipEntity::VisualType::Androsynth,           "Androsynth" },
        { ShipEntity::VisualType::AndrosynthBlazer,     "Androsynth" },
        { ShipEntity::VisualType::Arilou,               "Arilou" },
        { ShipEntity::VisualType::Human,                "Human" },
    };
}

SFMLShipHudTileRenderer::SFMLShipHudTileRenderer(ShipEntity& entity, const sf::FloatRect& renderArea)
	: m_entity(entity)
    , m_renderArea(renderArea)
    , m_font(SFMLAssetLoader::Instance().getFont("8bitoperatorplus8-bold"))
{

}

void SFMLShipHudTileRenderer::render(sf::RenderTarget& renderer)
{
    sf::RectangleShape r;
    r.setFillColor(sf::Color(150, 150, 150));
    r.setPosition(m_renderArea.position);
    r.setSize(m_renderArea.size);
    renderer.draw(r);

    sf::Text text(m_font->font, kRaceNames.at(m_entity.visualType()));
    text.setPosition(sf::Vector2f(m_renderArea.position.x + m_renderArea.size.x / 2, m_renderArea.position.y + 10));
    text.setCharacterSize(22);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y));
    renderer.draw(text);

    const float healthNormalized = m_entity.health() / (float)m_entity.maxHealth();
    drawVerticalBarGraph(renderer, sf::FloatRect( sf::Vector2f(m_renderArea.position.x + 30, m_renderArea.position.y + 40), sf::Vector2f(20, 100) ), kLifeBarInactiveFillColour, kLifeBarActiveFillColour, healthNormalized);

    {
        sf::Text text(m_font->font, "Health");
        text.setPosition(sf::Vector2f(m_renderArea.position.x + 40, m_renderArea.position.y + 145));
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(16);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y));
        renderer.draw(text);
    }

    const float energyNormalized = m_entity.energy() / (float)m_entity.maxEnergy();
    drawVerticalBarGraph(renderer, sf::FloatRect(sf::Vector2f(m_renderArea.position.x + m_renderArea.size.x - 30 - 20, m_renderArea.position.y + 40), sf::Vector2f(20, 100) ), kEnergyBarInactiveFillColour, kEnergyBarActiveFillColour, energyNormalized);

    {
        sf::Text text(m_font->font, "Energy");
        text.setPosition(sf::Vector2f(m_renderArea.position.x + m_renderArea.size.x - 30 - 10, m_renderArea.position.y + 145));
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(16);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y));
        renderer.draw(text);
    }
}

void SFMLShipHudTileRenderer::drawVerticalBarGraph(sf::RenderTarget& renderer, sf::FloatRect area, sf::Color inactiveColour, sf::Color activeColour, float valueNormalized)
{
    // Background fill
    {
        sf::RectangleShape bg;
        bg.setFillColor(inactiveColour);
        bg.setPosition(area.position);
        bg.setSize(area.size);
        renderer.draw(bg);
    }

    // Foreground fill
    {
        sf::RectangleShape fg;
        fg.setFillColor(activeColour);
        fg.setPosition(sf::Vector2f(area.position.x, area.position.y + area.size.y * (1 - valueNormalized)));
        fg.setSize(sf::Vector2f(area.size.x, area.size.y * valueNormalized));
        renderer.draw(fg);
    }
}
