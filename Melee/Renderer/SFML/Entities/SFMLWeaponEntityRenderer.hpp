#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"
#include "Renderer/SFML/SFMLAssetLoader.hpp"

namespace Melee
{
    class WeaponEntity;

    class SFMLWeaponEntityRenderer : public SFMLRenderable
    {
    public:
        explicit                        SFMLWeaponEntityRenderer(WeaponEntity& entity);
        virtual                         ~SFMLWeaponEntityRenderer() = default;

        // SFMLRenderable i/f:
    public:
        void                            render(sf::RenderTarget& renderer) override;

    private:
        const WeaponEntity&             m_entity;

        SFMLAssetLoader::CachedTexture  m_weaponImage;
        sf::Sprite                      m_sprite;
    };
}
