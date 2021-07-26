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

    template <typename EntityType, typename RenderContextType>
    inline auto CreateEntityRenderContext(const std::shared_ptr<Entity>& entity)
    {
        const auto specificEntity = std::dynamic_pointer_cast<EntityType>(entity);
        return std::make_shared<RenderContextType>(*specificEntity);
    }
}
