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

        uint32_t                    age() const                 { return m_age; }

    // Entity i/f:
    public:
        const WeaponProperties&     properties() const override { return m_weaponProperties; }

        void                        update(Engine& engine, uint32_t msElapsed) override;
        void                        collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState) override;

    private:
        const WeaponProperties      m_weaponProperties;

        uint32_t                    m_age = 0;
    };
}
