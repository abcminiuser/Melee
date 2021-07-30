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

void SFMLExplosionEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto explosionPos = m_entity.position();
    const auto explosionAge = m_entity.age();
    const auto explosionMaxAge = m_entity.properties().maxAge_ms;
    const auto explosionRadius = m_entity.properties().radius_km;

    const auto ageNormalized = explosionAge / static_cast<float>(explosionMaxAge);

    const auto currentRadius = LinearInterpolate(0.0f, explosionRadius, ageNormalized);
    const auto explosionColour = kExplosionColours[LinearInterpolateIndex(kExplosionColours, ageNormalized)];

    m_shape.setRadius(currentRadius);
    m_shape.setOrigin(currentRadius, currentRadius);

    m_shape.setFillColor(explosionColour);
    m_shape.setPosition(ToSFMLVector(explosionPos));

    renderer.draw(m_shape);
}
