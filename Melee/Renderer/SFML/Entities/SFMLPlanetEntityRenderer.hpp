#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"
#include "Renderer/SFML/SFMLAssetLoader.hpp"

namespace Melee
{
    class PlanetEntity;

    class SFMLPlanetEntityRenderer : public SFMLRenderable
    {
    public:
        explicit                        SFMLPlanetEntityRenderer(PlanetEntity& entity);
        virtual                         ~SFMLPlanetEntityRenderer() = default;

        // SFMLRenderable i/f:
    public:
        void                            render(sf::RenderTarget& renderer) override;

    private:
        const PlanetEntity&             m_entity;

        uint32_t                        m_lastScaleFactor = {};

        SFMLAssetLoader::CachedTexture  m_planetImage;
        sf::Sprite                      m_sprite;
    };
}
