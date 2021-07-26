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

    private:
        void                            updatePlayfieldViewport();
        void                            processEvents(sf::RenderWindow& window);
        void                            handleKey(sf::Keyboard::Key key, bool down);
        void                            renderEntities(sf::RenderTarget& target);

        std::shared_ptr<RenderContext>  createRenderContext(const std::shared_ptr<Entity>& entity);

    private:
        Engine&                         m_engine;

        sf::View                        m_playfieldView;
    };

    using Renderer = SFMLRenderer;
}
