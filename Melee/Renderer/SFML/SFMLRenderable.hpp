#pragma once

#include <SFML/Graphics.hpp>

#include "Engine/EngineTypes.hpp"

#include <memory>

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
        std::unique_ptr<SFMLRenderable> playfieldRenderer;
        std::unique_ptr<SFMLRenderable> uiRenderer;
    };
}
