#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"
#include "Renderer/SFML/SFMLAssetLoader.hpp"

namespace Melee
{
	class ShipEntity;

	class SFMLShipHudTileRenderer : public SFMLRenderable
	{
	public:
		explicit					SFMLShipHudTileRenderer(ShipEntity& entity, const sf::FloatRect& renderArea);
		virtual						~SFMLShipHudTileRenderer() = default;

	    // SFMLRenderable i/f:
	public:
	    void						render(sf::RenderTarget& renderer) override;

	private:
		void						drawVerticalBarGraph(sf::RenderTarget& renderer, sf::FloatRect area, sf::Color inactiveColour, sf::Color activeColour, float valueNormalized);

	private:
		const ShipEntity&			m_entity;
		const sf::FloatRect			m_renderArea;

	    SFMLAssetLoader::CachedFont	m_font;
	};
}
