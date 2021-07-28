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
        void                            renderPlayfield(sf::RenderTarget& target);
        void                            renderPlayerHud(sf::RenderTarget& target);

        std::shared_ptr<RenderContext>  createEntityRenderContext(const std::shared_ptr<Entity>& entity);

    private:
        Engine&                         m_engine;

        sf::Texture                     m_backgroundTexture;
        sf::Sprite                      m_backgroundSprite;

        sf::View                        m_playfieldView;
        sf::View                        m_playerHudView;
    };

    using Renderer = SFMLRenderer;
}
