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
        	Androsynth,
        	AndrosynthBlazer,
            Arilou,
            Human,
        };

        struct ShipProperties : public Properties
        {
            VisualType  visualType            = VisualType::Human;

            float       engineForce_N         = 0;
            float       rotation_degPerSec    = 0;

            uint32_t    maxHealth             = 1;
            uint32_t    maxEnergy             = 10;

            uint32_t    energyRechargeRate_ms = 1000;

            uint32_t    primaryFireRate_ms    = 200;
            uint32_t    primaryEnergyCost     = 1;

            uint32_t    specialFireRate_ms    = 1000;
            uint32_t    specialEnergyCost     = 5;

            bool        generatesExhaust      = true;
        };

    public:
        explicit                    ShipEntity(const ShipProperties& properties, const Point& position);
        virtual                     ~ShipEntity() = default;

        void                        handleKey(KeyEvent key, bool down);

        VisualType                  visualType() const          { return m_visualType; }

        auto                        maxHealth() const           { return m_maxHealth; }
        auto                        maxEnergy() const           { return m_maxEnergy; }

        uint32_t                    health() const              { return m_health; }
        uint32_t                    energy() const              { return m_energy; }

        void                        applyDamage(int amount);
        void                        consumeEnergy(int amount);

    // Entity i/f:
    public:
        void                        update(Engine& engine, uint32_t msElapsed) override;
        void                        collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState) override;

    protected:
        virtual bool                onPrimaryWeaponFired(Engine& engine) = 0;
        virtual bool                onSpecialWeaponFired(Engine& engine) = 0;
        virtual void                onControlFlagsUpdated() {}

    protected:
        struct ControlFlags
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

        VisualType                  m_visualType = VisualType::Human;
        float                       m_engineForce_N = 0;
        float                       m_rotation_degPerSec = 0;
        uint32_t                    m_maxHealth = 0;
        uint32_t                    m_maxEnergy = 0;
        uint32_t                    m_energyRechargeRate_ms = 0;
        uint32_t                    m_primaryEnergyCost = 0;
        uint32_t                    m_specialEnergyCost = 0;
        bool                        m_generatesExhaust = true;

        float                       m_engineAcceleration_ms2 = 0;
        Matrix2x2                   m_rotationalThrustLeft;
        Matrix2x2                   m_rotationalThrustRight;

        std::bitset<ControlFlags::_Count>  m_controlFlags;

        Periodic                    m_energyRechargeTimer;
        Periodic                    m_rotationTimer;
        Periodic                    m_thrustExhaustTimer;
        Periodic                    m_primaryFireTimer;
        Periodic                    m_specialFireTimer;
        Periodic                    m_thrustDelayTimer;

        uint32_t                    m_health = 0;
        uint32_t                    m_energy = 0;
    };
}
