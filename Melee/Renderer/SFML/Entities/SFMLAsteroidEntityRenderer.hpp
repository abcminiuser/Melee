#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"
#include "Renderer/SFML/SFMLAssetLoader.hpp"

namespace Melee
{
    class AsteroidEntity;

    class SFMLAsteroidEntityRenderer : public SFMLRenderable
    {
    public:
        explicit                        SFMLAsteroidEntityRenderer(AsteroidEntity& entity);
        virtual                         ~SFMLAsteroidEntityRenderer() = default;

        // SFMLRenderable i/f:
    public:
        void                            render(sf::RenderTarget& renderer) override;

    private:
        const AsteroidEntity&           m_entity;

        Vector2d                        m_lastHeading = {};

        SFMLAssetLoader::CachedTexture  m_asteroidImage;
        sf::Sprite                      m_sprite;
    };
}
