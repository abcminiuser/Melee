#include "SFMLAsteroidEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

SFMLAsteroidEntityRenderer::SFMLAsteroidEntityRenderer(AsteroidEntity& entity)
    : m_entity(entity)
    , m_asteroidImage(SFMLAssetLoader::Instance().getTexture("asteroid"))
{
    m_sprite.setTexture(m_asteroidImage->texture);
    m_sprite.setTextureRect(m_asteroidImage->region);
    m_sprite.setOrigin(sf::Vector2f{ m_asteroidImage->region.width / 2.0f, m_asteroidImage->region.height / 2.0f });
}

void SFMLAsteroidEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto asteroidRadius = m_entity.radius();
    const auto asteroidHeading = m_entity.heading();
    const auto asteroidPos = m_entity.position();

    const auto scaleFactor = asteroidRadius * 2 / std::max(m_asteroidImage->region.width, m_asteroidImage->region.height);

    m_sprite.setRotation(ToDegrees(asteroidHeading));
    m_sprite.setScale(sf::Vector2f{ scaleFactor, scaleFactor });
    m_sprite.setPosition(ToSFMLVector(asteroidPos));

    renderer.draw(m_sprite);
}
