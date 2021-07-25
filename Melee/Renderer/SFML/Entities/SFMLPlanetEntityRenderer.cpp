#include "SFMLPlanetEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

SFMLPlanetEntityRenderer::SFMLPlanetEntityRenderer(PlanetEntity& entity)
    : m_entity(entity)
{
    m_planetImage.loadFromFile("Assets\\Planet.png");
    m_planetImageSize = m_planetImage.getSize();

    m_sprite.setTexture(m_planetImage);
    m_sprite.setOrigin(sf::Vector2f{ m_planetImageSize.x / 2.0f, m_planetImageSize.y / 2.0f });
}

void SFMLPlanetEntityRenderer::render(sf::RenderTarget& renderer, uint32_t scaleFactor)
{
    const auto planetRadius = m_entity.properties().radius_km / scaleFactor;
    const auto planetPos = m_entity.position() / scaleFactor;

    if (m_lastScaleFactor != scaleFactor)
    {
        m_lastScaleFactor = scaleFactor;

        m_sprite.setScale(sf::Vector2f{ planetRadius * 2 / m_planetImageSize.x, planetRadius * 2 / m_planetImageSize.y });
    }

    m_sprite.setPosition(ToSFMLVector(planetPos));

    renderer.draw(m_sprite);
}
