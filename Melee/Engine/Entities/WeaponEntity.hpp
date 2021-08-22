#pragma once

#include "Engine/Entity.hpp"
#include "Engine/Util/Periodic.hpp"

namespace Melee
{
    class WeaponEntity : public Entity
    {
    public:
        enum class VisualType
        {
            Laser,
            HumanMissile,
        };

        struct WeaponProperties : public Properties
        {
            WeaponProperties()
            {
                collidable = true;
                wrappable = false;
                radius_km = 200;
                mass_kg = 1000;
            }

            VisualType visualType = VisualType::Laser;
            uint32_t   firingForce_N = 20000;
            uint32_t   maxAge_ms = 2000;
            bool       homing = false;
            bool       linger = false;
            float      rotation_degPerSec = 0;
        };

    public:
        explicit                    WeaponEntity(const std::shared_ptr<Entity>& parent, const WeaponProperties& properties, const Point& position, const Vector2d& velocity = Vector2d{}, const Vector2d& heading = Vector2d{});
        virtual                     ~WeaponEntity() = default;

        VisualType                  visualType() const          { return m_visualType; }

        auto                        maxAge() const              { return m_maxAge_ms; }

        uint32_t                    age() const { return m_age; }

    // Entity i/f:
    public:
        void                        update(Engine& engine, uint32_t msElapsed) override;
        void                        collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState) override;

    private:
        VisualType                  m_visualType = VisualType::Laser;

        uint32_t                    m_maxAge_ms = 0;
        bool                        m_homing = false;
        bool                        m_linger = false;
        float                       m_rotation_degPerSec = 0;

        uint32_t                    m_age = 0;

        Matrix2x2                   m_rotationalThrustLeft;
        Matrix2x2                   m_rotationalThrustRight;

        Periodic                    m_rotationTimer;

        std::weak_ptr<Entity>       m_lockedTarget;
    };
}
