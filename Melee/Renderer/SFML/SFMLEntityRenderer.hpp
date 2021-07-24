#pragma once

#include <SFML/Graphics.hpp>

namespace Melee
{
	class Engine;

	namespace SFMLEntityRenderer
	{
		void RenderEntites(Engine& engine, sf::RenderWindow& window, uint32_t scaleFactor);
	}
}
