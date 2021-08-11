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
                wrappable = false;
                radius_km = 2500;
            }

            uint32_t maxAge_ms = 1000;
            float    subExplosionProbability = 1.0f;
        };

    public:
        explicit    ExplosionEntity(const std::shared_ptr<Entity>& parent, const ExplosionProperties& properties, const Point& position);
        virtual     ~ExplosionEntity() = default;

        auto        maxAge() const      { return m_maxAge_ms; }

        uint32_t    age() const         { return m_age; }

    // Entity i/f:
    public:
        void        update(Engine& engine, uint32_t msElapsed) override;

    private:
        uint32_t    m_maxAge_ms = 0;
        float       m_subExplosionProbability = 0;

        uint32_t    m_age = 0;
        bool        m_subExplosionOccured = false;
    };
}
