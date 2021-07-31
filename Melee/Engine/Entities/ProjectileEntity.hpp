#pragma once

#include "Engine/Entity.hpp"

namespace Melee
{
    class ProjectileEntity : public Entity
    {
    public:
        struct ProjectileProperties : public Properties
        {
            ProjectileProperties()
            {
                wrappable = false;
                radius_km = 200;
                mass_kg = 1000;
            }

            uint32_t firingForce_N = 20000;
            uint32_t maxAge_ms = 2000;
            uint8_t  damage = 1;
        };

    public:
        explicit    					ProjectileEntity(const std::shared_ptr<Entity>& parent, const ProjectileProperties& properties, const Point& position, const Vector2d& velocity, const Vector2d& heading);
        virtual     					~ProjectileEntity() = default;

        uint32_t    					age() const         		{ return m_age; }

    // Entity i/f:
    public:
        const ProjectileProperties&		properties() const override	{ return m_projectileProperties; }

        void        					update(Engine& engine, uint32_t msElapsed) override;
        void        					collide(Engine& engine, const Entity& otherEntity, const PreCollisionState& otherEntityState) override;

    private:
        const ProjectileProperties      m_projectileProperties;

        uint32_t                        m_age = 0;
    };
}
