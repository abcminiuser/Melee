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
        explicit                	PlanetEntity(const PlanetProperties& properties, const Point& position);
        virtual                 	~PlanetEntity() = default;

    // Entity i/f:
    public:
        const PlanetProperties&		properties() const override	{ return m_planetProperties; }

        void                    	update(Engine& engine, uint32_t msElapsed) override;

    private:
        const PlanetProperties		m_planetProperties;
    };
}
