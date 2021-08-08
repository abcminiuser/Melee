#pragma once

#include "Engine/Entity.hpp"
#include "Engine/Util/Periodic.hpp"

#include <bitset>

namespace Melee
{
    class ShipEntity : public Entity
    {
    public:
        enum class KeyEvent
        {
            Thrust,
            RotateLeft,
            RotateRight,
            ReverseThrust,
            FirePrimary,
            FireSpecial,
        };

        enum class VisualType
        {
            Race1Ship,
            Race2Ship,
        };

        struct ShipProperties : public Properties
        {
            VisualType  visualType            = VisualType::Race1Ship;

            float       engineForce_N         = 0;
            float       rotation_degPerSec    = 0;

            uint32_t    maxHealth             = 1;
            uint32_t    maxEnergy             = 10;

            uint32_t    energyRechargeRate_ms = 1000;

            uint32_t    primaryFireRate_ms    = 200;
            uint32_t    primaryEnergyCost     = 1;
        };

    public:
        explicit    				ShipEntity(const ShipProperties& properties, const Point& position);
        virtual     				~ShipEntity() = default;

        void       					handleKey(KeyEvent key, bool down);

        uint32_t   					health() const      		{ return m_health; }
        uint32_t   					energy() const      		{ return m_energy; }

    // Entity i/f:
    public:
        const ShipProperties&		properties() const override	{ return m_shipProperties; }

        void       					update(Engine& engine, uint32_t msElapsed) override;
        void       					collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState) override;

    protected:
        virtual void                onPrimaryWeaponFired(Engine& engine) = 0;

        void       					applyDamage(int amount);
        void       					consumeEnergy(int amount);

    protected:
        struct Flags
        {
            enum
            {
                ThrustActive,
                ReverseThrustActive,
                RotateLeftActive,
                RotateRightActive,
                FirePrimaryActive,
                FireSpecialActive,

                _Count
            };
        };

        const ShipProperties  		m_shipProperties;

        float                   	m_engineAcceleration_ms2;
        Matrix2x2               	m_rotationalThrustLeft;
        Matrix2x2               	m_rotationalThrustRight;

        std::bitset<Flags::_Count>  m_flags;

        Periodic                	m_energyRechargeTimer;
        Periodic                	m_rotationTimer;
        Periodic                	m_thrustExhaustTimer;
        Periodic                	m_primaryFireTimer;

        uint32_t                	m_health = 0;
        uint32_t                	m_energy = 0;
    };
}
