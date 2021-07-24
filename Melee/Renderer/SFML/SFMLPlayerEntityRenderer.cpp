#include "SFMLPlayerEntityRenderer.hpp"
#include "SFMLUtils.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

SFMLPlayerEntityRenderer::SFMLPlayerEntityRenderer(PlayerEntity& entity)
	: m_entity(entity)
{
	static const sf::Color kShipColours[] =
		{
		    { 255,   0,   0 },
		    {   0, 255,   0 },
		    { 255,   0, 255 },
		};

    const int playerIndex = m_entity.index();

	m_shape.setFillColor(kShipColours[playerIndex % std::size(kShipColours)]);
	m_shape.setPointCount(3);
}

void SFMLPlayerEntityRenderer::render(sf::RenderWindow& window, uint32_t scaleFactor)
{
    static const auto kRotateLeft90 = RotationMatrix(-90);
    static const auto kRotateRight90 = RotationMatrix(90);

    const int playerIndex = m_entity.index();
    const auto playerHeading = m_entity.heading();
    const auto playerPos = m_entity.position() / scaleFactor;
    const auto playerRadius = m_entity.properties().radius_km / scaleFactor;

    m_shape.setPoint(0, ToSFML(playerPos + playerHeading * playerRadius));
    m_shape.setPoint(1, ToSFML(playerPos + kRotateLeft90 * playerHeading * playerRadius));
    m_shape.setPoint(2, ToSFML(playerPos + kRotateRight90 * playerHeading * playerRadius));

    window.draw(m_shape);
}
