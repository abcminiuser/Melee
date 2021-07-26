#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"

namespace Melee
{
    class PlanetEntity;

    class SFMLPlanetEntityRenderer : public SFMLRenderable
    {
    public:
        explicit            SFMLPlanetEntityRenderer(PlanetEntity& entity);
        virtual             ~SFMLPlanetEntityRenderer() = default;

        // SFMLRenderable i/f:
    public:
        void                render(sf::RenderTarget& renderer) override;

    private:
        const PlanetEntity& m_entity;

        uint32_t            m_lastScaleFactor = {};

        sf::Texture         m_planetImage;
        sf::Vector2u        m_planetImageSize;
        sf::Sprite          m_sprite;
    };
}
