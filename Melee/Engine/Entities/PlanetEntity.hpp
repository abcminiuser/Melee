#pragma once

#include "Engine/Entity.hpp"

namespace Melee
{
    class PlanetEntity : public Entity
    {
    public:
        struct PlanetProperties : public Properties
        {

        };

    public:
        explicit                	PlanetEntity(const PlanetProperties& properties, const Point& pos);
        virtual                 	~PlanetEntity() = default;

    // Entity i/f:
    public:
        const PlanetProperties&		properties() const override	{ return m_planetProperties; }

        void                    	update(Engine& engine, uint32_t msElapsed) override;

    private:
        const PlanetProperties		m_planetProperties;
    };
}
