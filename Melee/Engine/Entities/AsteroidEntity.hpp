#pragma once

#include "Engine/Entity.hpp"

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
        explicit    AsteroidEntity(const AsteroidProperties& properties, const Point& pos, const Vector2d& heading);
        virtual     ~AsteroidEntity() = default;

        const auto& properties() const  { return m_asteroidProperties; }

        // Entity i/f:
    public:
        void        update(Engine& engine, uint32_t msElapsed) override;
        void        collide(Engine& engine, const Entity& otherEntity) override;

    private:
        const AsteroidProperties    m_asteroidProperties;

        uint32_t                    m_rotationMsElapsed = 0;
        Matrix2x2                   m_rotation;
    };
}
