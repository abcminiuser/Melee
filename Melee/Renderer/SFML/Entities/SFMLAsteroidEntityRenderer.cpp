#include "SFMLAsteroidEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

SFMLAsteroidEntityRenderer::SFMLAsteroidEntityRenderer(AsteroidEntity& entity)
    : m_entity(entity)
{
    m_asteroidImage.loadFromFile("Assets\\Asteroid.png");
    m_asteroidImageSize = m_asteroidImage.getSize();

    m_sprite.setTexture(m_asteroidImage);
    m_sprite.setOrigin(sf::Vector2f{ m_asteroidImageSize.x / 2.0f, m_asteroidImageSize.y / 2.0f });
}

void SFMLAsteroidEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto asteroidRadius = m_entity.properties().radius_km;
    const auto asteroidHeading = m_entity.heading();
    const auto asteroidPos = m_entity.position();

    m_sprite.setRotation(ToDegrees(asteroidHeading));
    m_sprite.setScale(sf::Vector2f{ asteroidRadius * 2 / m_asteroidImageSize.x, asteroidRadius * 2 / m_asteroidImageSize.y });
    m_sprite.setPosition(ToSFMLVector(asteroidPos));

    renderer.draw(m_sprite);
}
