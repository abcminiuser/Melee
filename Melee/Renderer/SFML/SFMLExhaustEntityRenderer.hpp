#pragma once

#include "SFMLRenderable.hpp"

namespace Melee
{
	class ExhaustEntity;

	class SFMLExhaustEntityRenderer : public SFMLRenderable
	{
	public:
		explicit				SFMLExhaustEntityRenderer(ExhaustEntity& entity);
		virtual					~SFMLExhaustEntityRenderer() = default;

		// SFMLRenderable i/f:
	public:
		void					render(sf::RenderWindow& window, uint32_t scaleFactor) override;

	private:
		const ExhaustEntity&	m_entity;

		sf::CircleShape			m_shape;
	};
}
