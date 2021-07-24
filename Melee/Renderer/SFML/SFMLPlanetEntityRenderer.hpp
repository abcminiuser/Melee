#pragma once

#include "SFMLRenderable.hpp"

namespace Melee
{
	class PlanetEntity;

	class SFMLPlanetEntityRenderer : public SFMLRenderable
	{
	public:
		explicit			SFMLPlanetEntityRenderer(PlanetEntity& entity);
		virtual				~SFMLPlanetEntityRenderer() = default;

		// SFMLRenderable i/f:
	public:
		void				render(sf::RenderWindow& window, uint32_t scaleFactor) override;

	private:
		const PlanetEntity&	m_entity;

		sf::CircleShape		m_shape;
	};
}
