#pragma once

#include "Engine/Entity.hpp"

namespace Melee
{
    class PlanetEntity : public Entity
    {
    public:
        enum class VisualType
        {
            Planet1,
            Planet2,
        };

        struct PlanetProperties : public Properties
        {
            VisualType visualType = VisualType::Planet1;
        };

    public:
        explicit        PlanetEntity(const PlanetProperties& properties, const Point& position);
        virtual         ~PlanetEntity() = default;

        VisualType      visualType() const  { return m_visualType; }

    // Entity i/f:
    public:
        void            update(Engine& engine, uint32_t msElapsed) override;

    private:
        VisualType      m_visualType = VisualType::Planet1;
    };
}
