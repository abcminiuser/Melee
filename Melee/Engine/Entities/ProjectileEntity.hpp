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
                radius_km = 200;
            }

            uint32_t maxAge_ms = 2000;
            uint8_t  damage = 1;
        };

    public:
        explicit    ProjectileEntity(const ProjectileProperties& properties, const std::shared_ptr<Entity>& ownerEntity, const Point& pos, const Vector2d& heading);
        virtual     ~ProjectileEntity() = default;

        const auto& properties() const  { return m_projectileProperties; }

        uint32_t    age() const         { return m_age; }
        auto        ownerEntity() const { return m_ownerEntity; }

    // Entity i/f:
    public:
        void        update(Engine& engine, uint32_t msElapsed) override;
        void        collide(Engine& engine, const Entity& otherEntity) override;

    private:
        const ProjectileProperties      m_projectileProperties;

        const std::shared_ptr<Entity>   m_ownerEntity;

        uint32_t                        m_age = 0;
    };
}
