#include "SFMLPlanetEntityRenderer.hpp"
#include "SFMLUtils.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

SFMLPlanetEntityRenderer::SFMLPlanetEntityRenderer(PlanetEntity& entity)
	: m_entity(entity)
{
	m_shape.setFillColor(sf::Color(100, 100, 200));
}

void SFMLPlanetEntityRenderer::render(sf::RenderWindow& window, uint32_t scaleFactor)
{
	const auto planetRadius = m_entity.properties().radius_km / scaleFactor;
	const auto planetPos = m_entity.position() / scaleFactor;

	m_shape.setPosition(ToSFML(planetPos));
	m_shape.setRadius(planetRadius);
	m_shape.setOrigin(planetRadius, planetRadius);

    window.draw(m_shape);
}
