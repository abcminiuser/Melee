#pragma once

#include <SFML/Graphics.hpp>

#include "Engine/EngineTypes.hpp"

namespace Melee
{
    class Entity;

    class RenderContext
    {
    public:
        explicit        RenderContext() = default;
        virtual         ~RenderContext() = default;

    public:
        virtual void    render(sf::RenderTarget& renderer) = 0;
    };

    using SFMLRenderable = RenderContext;
}
