#include "SFMLExhaustEntityRenderer.hpp"
#include "SFMLUtils.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

SFMLExhaustEntityRenderer::SFMLExhaustEntityRenderer(ExhaustEntity& entity)
	: m_entity(entity)
{
	m_shape.setRadius(1);
	m_shape.setOrigin(1, 1);
}

void SFMLExhaustEntityRenderer::render(sf::RenderTarget& window, uint32_t scaleFactor)
{
	static const sf::Color kExhaustColours[] =
		{
		    HSVColor(50, 100, 100),
		    HSVColor(40, 85, 80),
		    HSVColor(30, 85, 60),
		    HSVColor(20, 85, 40),
		    HSVColor(10, 85, 35),
		    HSVColor(0, 85, 20),
		};

	const auto exhaustPos = m_entity.position() / scaleFactor;
	const auto exhaustAge = m_entity.age();
	const auto exhaustMaxAge = m_entity.properties().maxAge;

	m_shape.setFillColor(kExhaustColours[exhaustAge * std::size(kExhaustColours) / exhaustMaxAge]);
	m_shape.setPosition(ToSFML(exhaustPos));

    window.draw(m_shape);
}
