#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLRenderable.hpp"

namespace Melee
{
    class Engine;

    class SFMLRenderer final
    {
    public:
        explicit                        SFMLRenderer(Engine& engine);
                                        ~SFMLRenderer() = default;

        int                             runModal();

        uint32_t                        currentScaleFactor() const { return 100; } // FIXME: Dynamic view scaling to fit the players

    private:
        void                            processEvents(sf::RenderWindow& window);
        void                            handleKey(sf::Keyboard::Key key, bool down);
        void                            renderEntities(sf::RenderWindow& window);

        std::shared_ptr<RenderContext>  createRenderContext(const std::shared_ptr<Entity>& entity);

    private:
        Engine&                         m_engine;
    };

    using Renderer = SFMLRenderer;
}
