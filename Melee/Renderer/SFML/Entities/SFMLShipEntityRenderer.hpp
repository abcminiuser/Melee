#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"
#include "Renderer/SFML/SFMLAssetLoader.hpp"

#include "Engine/Entities/ShipEntity.hpp"

namespace Melee
{
    class SFMLShipEntityRenderer : public SFMLRenderable
    {
    public:
        explicit                        SFMLShipEntityRenderer(ShipEntity& entity);
        virtual                         ~SFMLShipEntityRenderer() = default;

        void                            updateTexture();

        // SFMLRenderable i/f:
    public:
        void                            render(sf::RenderTarget& window) override;
 
    private:
        const ShipEntity&               m_entity;

        ShipEntity::VisualType          m_cachedVisualType = ShipEntity::VisualType::Human;
        SFMLAssetLoader::CachedTexture  m_shipImage;
        sf::Sprite                      m_sprite;
    };
}
