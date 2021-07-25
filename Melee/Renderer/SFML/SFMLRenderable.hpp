#pragma once

#include <SFML/Graphics.hpp>

#include "Engine/EngineTypes.hpp"

namespace Melee
{
	class Entity;

	class RenderContext
	{
	public:
		RenderContext() = default;
		virtual			~RenderContext() = default;

	public:
		virtual void	render(sf::RenderTarget& window, uint32_t scaleFactor) = 0;
	};

	using SFMLRenderable = RenderContext;
}
