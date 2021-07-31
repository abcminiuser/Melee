#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"
#include "Renderer/SFML/SFMLAssetLoader.hpp"

namespace Melee
{
	class PlayerEntity;

	class SFMLPlayerHudTileRenderer : public SFMLRenderable
	{
	public:
		explicit					SFMLPlayerHudTileRenderer(PlayerEntity& entity, const sf::FloatRect& renderArea);
		virtual						~SFMLPlayerHudTileRenderer() = default;

	    // SFMLRenderable i/f:
	public:
	    void						render(sf::RenderTarget& renderer) override;

	private:
		void						drawVerticalBarGraph(sf::RenderTarget& renderer, sf::FloatRect area, sf::Color inactiveColour, sf::Color activeColour, float valueNormalized);

	private:
		const PlayerEntity&			m_entity;
		const sf::FloatRect			m_renderArea;

	    SFMLAssetLoader::CachedFont	m_font;
	};
}
