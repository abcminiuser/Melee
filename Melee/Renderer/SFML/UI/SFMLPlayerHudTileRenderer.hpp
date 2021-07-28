#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"

namespace Melee
{
	class PlayerEntity;

	class SFMLPlayerHudTileRenderer : public SFMLRenderable
	{
	public:
		explicit				SFMLPlayerHudTileRenderer(PlayerEntity& entity, const sf::FloatRect& renderArea);
		virtual					~SFMLPlayerHudTileRenderer() = default;

	    // SFMLRenderable i/f:
	public:
	    void                    render(sf::RenderTarget& renderer) override;

	private:
		const PlayerEntity&		m_entity;
		const sf::FloatRect		m_renderArea;

	    sf::Font				m_font;
	};
}
