#pragma once

#include "SFMLRenderable.hpp"

namespace Melee
{
	class PlayerEntity;

	class SFMLPlayerEntityRenderer : public SFMLRenderable
	{
	public:
		explicit			SFMLPlayerEntityRenderer(PlayerEntity& entity);
		virtual				~SFMLPlayerEntityRenderer() = default;

		// SFMLRenderable i/f:
	public:
		void				render(sf::RenderWindow& window, uint32_t scaleFactor) override;

	private:
		const PlayerEntity&	m_entity;

		sf::ConvexShape		m_shape;
	};
}
