#pragma once

#include "SFMLRenderable.hpp"

namespace Melee
{
	class AsteroidEntity;

	class SFMLAsteroidEntityRenderer : public SFMLRenderable
	{
	public:
		explicit			SFMLAsteroidEntityRenderer(AsteroidEntity& entity);
		virtual				~SFMLAsteroidEntityRenderer() = default;

		// SFMLRenderable i/f:
	public:
		void				render(sf::RenderTarget& renderer, uint32_t scaleFactor) override;

	private:
		const AsteroidEntity&	m_entity;

		Vector2d				m_lastHeading = {};
		uint32_t				m_lastScaleFactor = {};

		sf::Texture				m_asteroidImage;
		sf::Vector2u			m_asteroidImageSize;
		sf::Sprite				m_sprite;
	};
}
