#include "ShipEntity.hpp"

#include "Engine/Engine.hpp"

#include <algorithm>

using namespace Melee;

namespace
{
    constexpr uint32_t kRotationIntervalMs = 10;
    constexpr uint32_t kThrustExhaustIntervalMs = 200;
}

ShipEntity::ShipEntity(const ShipProperties& properties, const Point& position)
    : Entity(Entity::Type::Ship, nullptr, properties, position)
    , m_visualType(properties.visualType)
    , m_engineForce_N(properties.engineForce_N)
    , m_rotation_degPerSec(properties.rotation_degPerSec)
    , m_maxHealth(properties.maxHealth)
    , m_maxEnergy(properties.maxEnergy)
    , m_energyRechargeRate_ms(properties.energyRechargeRate_ms)
    , m_primaryEnergyCost(properties.primaryEnergyCost)
    , m_specialEnergyCost(properties.specialEnergyCost)
    , m_energyRechargeTimer(properties.energyRechargeRate_ms)
    , m_rotationTimer(kRotationIntervalMs)
    , m_thrustExhaustTimer(kThrustExhaustIntervalMs, kThrustExhaustIntervalMs, true)
    , m_primaryFireTimer(properties.primaryFireRate_ms, properties.primaryFireRate_ms, true)
    , m_specialFireTimer(properties.specialFireRate_ms, properties.specialFireRate_ms, true)
    , m_thrustDelayTimer(300, 300, true)
{
    m_engineAcceleration_ms2    = properties.engineForce_N / properties.mass_kg;

    const float rotationDegreesPerInterval = properties.rotation_degPerSec * kRotationIntervalMs / 1000;
    m_rotationalThrustLeft      = RotationMatrix(-rotationDegreesPerInterval);
    m_rotationalThrustRight     = RotationMatrix(rotationDegreesPerInterval);

    m_health                    = properties.maxHealth;
    m_energy                    = properties.maxEnergy;
}

void ShipEntity::handleKey(KeyEvent key, bool down)
{
    if (key == KeyEvent::Thrust)
        m_controlFlags.set(ControlFlags::ThrustActive, down);
    else if (key == KeyEvent::ReverseThrust)
        m_controlFlags.set(ControlFlags::ReverseThrustActive, down);
    else if (key == KeyEvent::RotateLeft)
        m_controlFlags.set(ControlFlags::RotateLeftActive, down);
    else if (key == KeyEvent::RotateRight)
        m_controlFlags.set(ControlFlags::RotateRightActive, down);
    else if (key == KeyEvent::FirePrimary)
        m_controlFlags.set(ControlFlags::FirePrimaryActive, down);
    else if (key == KeyEvent::FireSpecial)
        m_controlFlags.set(ControlFlags::FireSpecialActive, down);

    onControlFlagsUpdated();
}

void ShipEntity::update(Engine& engine, uint32_t msElapsed)
{
    m_thrustDelayTimer.add(msElapsed);

    if (!m_health)
    {
        engine.removeEntity(shared_from_this());

        auto explosionEntity = std::make_shared<ExplosionEntity>(nullptr, ExplosionEntity::ExplosionProperties{}, m_position);
        engine.addEntity(explosionEntity);

        return;
    }

    if (m_controlFlags.test(ControlFlags::RotateLeftActive) || m_controlFlags.test(ControlFlags::RotateRightActive))
    {
        m_rotationTimer.add(msElapsed);

        const auto thrustRotationVector = m_controlFlags.test(ControlFlags::RotateLeftActive) ? m_rotationalThrustLeft : m_rotationalThrustRight;

        while (m_rotationTimer.expired())
            m_heading = thrustRotationVector * m_heading;
    }
    else
    {
        m_rotationTimer.reset();
    }

    if (m_controlFlags.test(ControlFlags::ThrustActive) || m_controlFlags.test(ControlFlags::ReverseThrustActive))
    {
        const auto thrustVector = m_controlFlags.test(ControlFlags::ThrustActive) ? m_heading : -m_heading;

        m_acceleration = thrustVector * m_engineAcceleration_ms2;

        m_thrustExhaustTimer.add(msElapsed);
        if (m_thrustDelayTimer.expired(false) && m_thrustExhaustTimer.expired())
        {
            if (m_generatesExhaust)
            {
                const auto spawnLocation = m_position + (-m_heading * (m_radius_km - 1));

                auto exhaustEntity = std::make_shared<ExhaustEntity>(shared_from_this(), ExhaustEntity::ExhaustProperties{}, spawnLocation, m_velocity);
                engine.addEntity(exhaustEntity, Engine::InsertionOrder::Bottom);
            }
        }
    }
    else
    {
        m_acceleration = Vector2d{ 0, 0 };
        m_thrustExhaustTimer.reset();
    }

    m_primaryFireTimer.add(msElapsed);
    if (m_controlFlags.test(ControlFlags::FirePrimaryActive) && m_energy >= m_primaryEnergyCost && m_primaryFireTimer.expired(false))
    {
        if (onPrimaryWeaponFired(engine))
        {
            consumeEnergy(m_primaryEnergyCost);
            m_primaryFireTimer.expired(true);
        }
    }

    m_specialFireTimer.add(msElapsed);
    if (m_controlFlags.test(ControlFlags::FireSpecialActive) && m_energy >= m_specialEnergyCost && m_specialFireTimer.expired(false))
    {
        if (onSpecialWeaponFired(engine))
        {
            consumeEnergy(m_specialEnergyCost);
            m_specialFireTimer.expired(true);
        }
    }

    m_energyRechargeTimer.add(msElapsed);
    while (m_energyRechargeTimer.expired())
        consumeEnergy(-1);

    Entity::update(engine, msElapsed);
}

void ShipEntity::collide(Engine& engine, const std::shared_ptr<Entity>& otherEntity, const PreCollisionState& otherEntityState)
{
    switch (otherEntity->type())
    {
        case Entity::Type::Asteroid:
        case Entity::Type::Planet:
        case Entity::Type::Ship:
        {
            // Bonk: colliding with these is an inelastic collision.

            m_acceleration = {};
            m_velocity = -m_velocity;

            m_thrustDelayTimer.reset();

            break;
        }

        case Entity::Type::Weapon:
        case Entity::Type::Exhaust:
        case Entity::Type::Explosion:
            break;
    }

    // Collisions will only damage us if:
    //   - The other entity's parent is not us, and
    //   - Our parent is not the other entity, and
    //   - We don't have a parent in common.
    const bool otherEntityParentIsUs = otherEntity->parentEntity() == shared_from_this();
    const bool ourParentIsTheOtherEntity = m_parentEntity == otherEntity;
    const bool parentsInCommon = m_parentEntity && (m_parentEntity == otherEntity->parentEntity());
    if (!otherEntityParentIsUs && !ourParentIsTheOtherEntity && !parentsInCommon)
        applyDamage(otherEntity->collisionDamage());

    Entity::collide(engine, otherEntity, otherEntityState);
}

void ShipEntity::applyDamage(int amount)
{
    m_health = std::clamp<int>(m_health - amount, 0, m_maxHealth);
}

void ShipEntity::consumeEnergy(int amount)
{
    m_energy = std::clamp<int>(m_energy - amount, 0, m_maxEnergy);
}
