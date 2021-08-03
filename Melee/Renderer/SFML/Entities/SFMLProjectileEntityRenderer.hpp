#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"
#include "Renderer/SFML/SFMLAssetLoader.hpp"

namespace Melee
{
    class ProjectileEntity;

    class SFMLProjectileEntityRenderer : public SFMLRenderable
    {
    public:
        explicit                        SFMLProjectileEntityRenderer(ProjectileEntity& entity);
        virtual                         ~SFMLProjectileEntityRenderer() = default;

        // SFMLRenderable i/f:
    public:
        void                            render(sf::RenderTarget& renderer) override;

    private:
        const ProjectileEntity&         m_entity;

        SFMLAssetLoader::CachedTexture  m_projectileImage;
        sf::Sprite                      m_sprite;
    };
}
