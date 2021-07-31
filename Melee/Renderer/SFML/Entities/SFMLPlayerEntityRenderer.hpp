#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"
#include "Renderer/SFML/SFMLAssetLoader.hpp"

namespace Melee
{
    class PlayerEntity;

    class SFMLPlayerEntityRenderer : public SFMLRenderable
    {
    public:
        explicit                        SFMLPlayerEntityRenderer(PlayerEntity& entity);
        virtual                         ~SFMLPlayerEntityRenderer() = default;

        // SFMLRenderable i/f:
    public:
        void                            render(sf::RenderTarget& window) override;

    private:
        const PlayerEntity&             m_entity;

        SFMLAssetLoader::CachedTexture  m_shipImage;
        sf::Sprite                      m_sprite;
    };
}
