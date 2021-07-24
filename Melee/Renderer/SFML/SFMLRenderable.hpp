#pragma once

#include <SFML/Graphics.hpp>

namespace Melee
{
	class Entity;

	class RenderContext
	{
	public:
		RenderContext() = default;
		virtual			~RenderContext() = default;

	public:
		virtual void	render(sf::RenderWindow& window, uint32_t scaleFactor) = 0;
	};

	using SFMLRenderable = RenderContext;
}
