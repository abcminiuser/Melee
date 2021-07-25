#pragma once

#include "Engine/Entity.hpp"
#include "Engine/Periodic.hpp"

namespace Melee
{
    class AsteroidEntity : public Entity
    {
    public:
        struct AsteroidProperties : public Properties
        {
            float   rotation_degPerSec = 0;
        };

    public:
        explicit    				AsteroidEntity(const AsteroidProperties& properties, const Point& pos, const Vector2d& heading);
        virtual     				~AsteroidEntity() = default;

        // Entity i/f:
    public:
        const AsteroidProperties&	properties() const override	{ return m_asteroidProperties; }

        void        				update(Engine& engine, uint32_t msElapsed) override;
        void        				collide(Engine& engine, const Entity& otherEntity, const PreCollisionState& otherEntityState) override;

    private:
        const AsteroidProperties    m_asteroidProperties;

        Periodic                    m_rotationTimer;
        Matrix2x2                   m_rotation;
    };
}
