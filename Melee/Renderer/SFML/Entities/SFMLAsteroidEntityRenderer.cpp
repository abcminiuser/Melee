#include "SFMLAsteroidEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

SFMLAsteroidEntityRenderer::SFMLAsteroidEntityRenderer(AsteroidEntity& entity)
    : m_entity(entity)
    , m_asteroidImage(SFMLAssetLoader::Instance().getTexture("asteroid-big-000"))
{
    m_sprite.setTexture(*m_asteroidImage->texture);
    m_sprite.setTextureRect(m_asteroidImage->region);
    m_sprite.setOrigin(ToOrigin(m_asteroidImage->region.width, m_asteroidImage->region.height));
}

void SFMLAsteroidEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto asteroidRadius = m_entity.radius();
    const auto asteroidHeading = m_entity.heading();
    const auto asteroidPos = m_entity.position();

    m_sprite.setRotation(ToDegrees(asteroidHeading));
    m_sprite.setScale(ToScaleFactor(asteroidRadius, m_asteroidImage->region.width, m_asteroidImage->region.height));
    m_sprite.setPosition(ToSFMLVector(asteroidPos));

    renderer.draw(m_sprite);
}
