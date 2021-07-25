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
		void				render(sf::RenderTarget& window, uint32_t scaleFactor) override;

	private:
		const PlayerEntity&	m_entity;

		Vector2d			m_lastHeading = {};
		uint32_t			m_lastScaleFactor = {};

		sf::Texture			m_shipImage;
		sf::Vector2u		m_shipImageSize;
		sf::Sprite			m_sprite;
	};
}
