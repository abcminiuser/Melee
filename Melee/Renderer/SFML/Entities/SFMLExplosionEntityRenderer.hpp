#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"

namespace Melee
{
	class ExplosionEntity;

	class SFMLExplosionEntityRenderer : public SFMLRenderable
	{
	public:
		explicit				SFMLExplosionEntityRenderer(ExplosionEntity& entity);
		virtual					~SFMLExplosionEntityRenderer() = default;

		// SFMLRenderable i/f:
	public:
		void					render(sf::RenderTarget& renderer, uint32_t scaleFactor) override;

	private:
		const ExplosionEntity&	m_entity;

		sf::CircleShape			m_shape;
	};
}
