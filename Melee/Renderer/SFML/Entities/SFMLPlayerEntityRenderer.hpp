#pragma once

#include "Renderer/SFML/SFMLRenderable.hpp"

namespace Melee
{
    class PlayerEntity;

    class SFMLPlayerEntityRenderer : public SFMLRenderable
    {
    public:
        explicit            SFMLPlayerEntityRenderer(PlayerEntity& entity);
        virtual             ~SFMLPlayerEntityRenderer() = default;

        // SFMLRenderable i/f:
    public:
        void                render(sf::RenderTarget& window) override;

    private:
        const PlayerEntity& m_entity;

        Vector2d            m_lastHeading = {};

        sf::Texture         m_shipImage;
        sf::Vector2u        m_shipImageSize;
        sf::Sprite          m_sprite;
    };
}
