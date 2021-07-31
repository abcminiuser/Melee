#pragma once

#include <SFML/Graphics.hpp>

#include "Engine/Engine.hpp"

#include "SFMLAssetLoader.hpp"
#include "SFMLRenderable.hpp"

namespace Melee
{
    class Engine;

    class SFMLRenderer : public Engine::Observer
    {
    public:
        explicit                        SFMLRenderer(Engine& engine);
        virtual		                    ~SFMLRenderer();

        int                             runModal();

    private:
        void                            updatePlayfieldViewport();
        void                            processEvents(sf::RenderWindow& window);
        void                            handleKey(sf::Keyboard::Key key, bool down);
        void                            renderPlayfield(sf::RenderTarget& target);
        void                            renderPlayerHud(sf::RenderTarget& target);

        std::shared_ptr<RenderContext>  getEntityRenderContext(const std::shared_ptr<Entity>& entity);

    private:
        Engine&                         m_engine;

        SFMLAssetLoader::CachedTexture  m_backgroundTexture;
        sf::Sprite                      m_backgroundSprite;

        sf::View                        m_playfieldView;
        sf::View                        m_playerHudView;
    };
}
