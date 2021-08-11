#pragma once

#include "Engine/Entity.hpp"

namespace Melee
{
    class WeaponEntity : public Entity
    {
    public:
        enum class VisualType
        {
            Race1Missile,
            Race1Mine,
            Race2Missile,
        };

        struct WeaponProperties : public Properties
        {
            WeaponProperties()
            {
                wrappable = false;
                radius_km = 200;
                mass_kg = 1000;
            }

            VisualType visualType = VisualType::Race1Missile;
            uint32_t   firingForce_N = 20000;
            uint32_t   maxAge_ms = 2000;
            uint8_t    damage = 1;
        };

    public:
        explicit                    WeaponEntity(const std::shared_ptr<Entity>& parent, const WeaponProperties& properties, const Point& position, const Vector2d& velocity = Vector2d{}, const Vector2d& heading = Vector2d{});
        virtual                     ~WeaponEntity() = default;

        VisualType                  visualType() const          { return m_visualType; }

        auto                        maxAge() const              { return m_maxAge_ms; }
        auto                        damage() const              { return m_damage; }

        uint32_t                    age() const { return m_age; }

    // Entity i/f:
    public:
        void                        update(Engine& engine, uint32_t msElapsed) override;
        void                        collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState) override;

    private:
        VisualType                  m_visualType = VisualType::Race1Missile;

        uint32_t                    m_maxAge_ms = 0;
        uint8_t                     m_damage = 0;

        uint32_t                    m_age = 0;
    };
}
