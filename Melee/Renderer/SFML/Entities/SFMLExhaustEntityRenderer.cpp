#include "SFMLExhaustEntityRenderer.hpp"

#include "Renderer/SFML/SFMLUtils.hpp"
#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    sf::CircleShape s_shape =
        []()
        {
            sf::CircleShape circle;
            circle.setRadius(120);
            circle.setOrigin(120, 120);

            return circle;
        }();

    const sf::Color kExhaustColours[] =
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

SFMLExhaustEntityRenderer::SFMLExhaustEntityRenderer(ExhaustEntity& entity)
    : m_entity(entity)
{

}

void SFMLExhaustEntityRenderer::render(sf::RenderTarget& renderer)
{
    const auto exhaustPos = m_entity.position();
    const auto exhaustAge = m_entity.age();
    const float exhaustMaxAge = m_entity.properties().maxAge_ms;

    const auto exhaustColour = kExhaustColours[LinearInterpolateIndex(kExhaustColours, exhaustAge / exhaustMaxAge)];

    s_shape.setFillColor(exhaustColour);
    s_shape.setPosition(ToSFMLVector(exhaustPos));

    renderer.draw(s_shape);
}
