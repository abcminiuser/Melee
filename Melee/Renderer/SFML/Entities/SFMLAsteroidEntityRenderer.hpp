#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"

namespace Melee
{
    class AsteroidEntity;

    class SFMLAsteroidEntityRenderer : public SFMLRenderable
    {
    public:
        explicit            SFMLAsteroidEntityRenderer(AsteroidEntity& entity);
        virtual             ~SFMLAsteroidEntityRenderer() = default;

        // SFMLRenderable i/f:
    public:
        void                render(sf::RenderTarget& renderer) override;

    private:
        const AsteroidEntity&   m_entity;

        Vector2d                m_lastHeading = {};

        sf::Texture             m_asteroidImage;
        sf::Vector2u            m_asteroidImageSize;
        sf::Sprite              m_sprite;
    };
}
