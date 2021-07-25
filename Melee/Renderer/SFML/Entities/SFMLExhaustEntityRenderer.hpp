#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"

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
		void					render(sf::RenderTarget& renderer, uint32_t scaleFactor) override;

	private:
		const ExhaustEntity&	m_entity;
	};
}
