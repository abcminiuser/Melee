#pragma once

#include <SFML/Graphics.hpp>

#include "Engine/EngineTypes.hpp"

namespace Melee
{
    class Entity;

    class SFMLRenderable
    {
    public:
        explicit        SFMLRenderable() = default;
        virtual         ~SFMLRenderable() = default;

    public:
        virtual void    render(sf::RenderTarget& renderer) = 0;
    };

    struct RenderContext
    {
        std::shared_ptr<SFMLRenderable> playfieldRenderer;
        std::shared_ptr<SFMLRenderable> uiRenderer;
    };
}
