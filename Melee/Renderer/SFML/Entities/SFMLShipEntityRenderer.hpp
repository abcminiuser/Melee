#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"
#include "Renderer/SFML/SFMLAssetLoader.hpp"

namespace Melee
{
    class ShipEntity;

    class SFMLShipEntityRenderer : public SFMLRenderable
    {
    public:
        explicit                        SFMLShipEntityRenderer(ShipEntity& entity);
        virtual                         ~SFMLShipEntityRenderer() = default;

        // SFMLRenderable i/f:
    public:
        void                            render(sf::RenderTarget& window) override;

    private:
        const ShipEntity&               m_entity;

        SFMLAssetLoader::CachedTexture  m_shipImage;
        sf::Sprite                      m_sprite;
    };
}
