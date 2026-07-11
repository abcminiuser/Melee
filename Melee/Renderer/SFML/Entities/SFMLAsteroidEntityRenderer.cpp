#include "SFMLAsteroidEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

SFMLAsteroidEntityRenderer::SFMLAsteroidEntityRenderer(AsteroidEntity& entity)
    : m_entity(entity)
    , m_asteroidImage(SFMLAssetLoader::Instance().getTexture("asteroid-big-000"))
    , m_sprite(*m_asteroidImage->texture, m_asteroidImage->region)
{
    m_sprite.setOrigin(ToOrigin(m_asteroidImage->region.size.x, m_asteroidImage->region.size.y));
}

void SFMLAsteroidEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto asteroidRadius = m_entity.radius();
    const auto asteroidHeading = m_entity.heading();
    const auto asteroidPos = m_entity.position();

    m_sprite.setRotation(sf::degrees(ToDegrees(asteroidHeading)));
    m_sprite.setScale(ToScaleFactor(asteroidRadius, m_asteroidImage->region.size.x, m_asteroidImage->region.size.y));
    m_sprite.setPosition(ToSFMLVector(asteroidPos));

    renderer.draw(m_sprite);
}
