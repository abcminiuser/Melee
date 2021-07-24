#pragma once

#include <SFML/Graphics.hpp>

namespace Melee
{
	class Engine;

	class SFMLRenderer final
	{
	public:
        explicit            SFMLRenderer(Engine& engine);
        					~SFMLRenderer() = default;

        int                 runModal();

        uint32_t            currentScaleFactor() const { return 100; } // FIXME: Dynamic view scaling to fit the players

    private:
        void                handleKey(sf::Keyboard::Key key, bool down);
        void                renderEntites(sf::RenderWindow& window);

    private:
        Engine&             m_engine;
	};
}
