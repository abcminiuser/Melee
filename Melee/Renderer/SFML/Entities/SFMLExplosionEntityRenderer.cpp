#include "SFMLExplosionEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    const sf::Color kExplosionColours[] =
        {
            HSVColor(  0,   0, 100),
            HSVColor( 50, 100, 100),
            HSVColor( 40,  85,  80),
            HSVColor( 30,  85,  60),
            HSVColor( 20,  85,  40),
            HSVColor( 10,  85,  35),
            HSVColor( 0 ,  85,  20),
        };
}

SFMLExplosionEntityRenderer::SFMLExplosionEntityRenderer(ExplosionEntity& entity)
    : m_entity(entity)
{

}

void SFMLExplosionEntityRenderer::render(sf::RenderTarget& renderer, uint32_t scaleFactor)
{
    const auto explosionPos = m_entity.position() / scaleFactor;
    const auto explosionAge = m_entity.age();
    const float explosionMaxAge = m_entity.properties().maxAge_ms;
    const float explosionRadius = m_entity.properties().radius_km / scaleFactor;

    const auto currentRadius = LinearInterpolate(0.0f, explosionRadius, explosionAge / explosionMaxAge);
    const auto explosionColour = kExplosionColours[LinearInterpolateIndex(kExplosionColours, explosionAge / explosionMaxAge)];

    m_shape.setRadius(currentRadius);
    m_shape.setOrigin(currentRadius, currentRadius);

    m_shape.setFillColor(explosionColour);
    m_shape.setPosition(ToSFMLVector(explosionPos));

    renderer.draw(m_shape);
}
