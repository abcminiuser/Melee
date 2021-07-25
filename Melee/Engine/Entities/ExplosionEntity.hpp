#pragma once

#include "Engine/Entity.hpp"

namespace Melee
{
    class ExplosionEntity : public Entity
    {
    public:
        struct ExplosionProperties : public Properties
        {
            ExplosionProperties()
            {
                collidable = false;
                radius_km = 2500;
            }

            uint32_t maxAge = 1000;
        };

    public:
        explicit    ExplosionEntity(const ExplosionProperties& properties, const Point& pos);
        virtual     ~ExplosionEntity() = default;

        const auto& properties() const  { return m_explosionProperties; }

        uint32_t    age() const         { return m_age; }

    // Entity i/f:
    public:
        void        update(Engine& engine, uint32_t msElapsed) override;

    private:
        const ExplosionProperties	m_explosionProperties;

        uint32_t                	m_age = 0;
    };
}
