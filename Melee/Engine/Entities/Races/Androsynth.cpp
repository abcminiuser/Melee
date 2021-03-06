#include "Androsynth.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

AndrosynthShipEntity::AndrosynthShipEntity(const Point& position)
    : ShipEntity(MakeShipProperties(), position)
    , m_blazerEnergyRequired(100)
{

}

void AndrosynthShipEntity::update(Engine& engine, uint32_t msElapsed)
{
    ShipEntity::update(engine, msElapsed);

    if (m_blazerMode)
    {
        m_blazerEnergyRequired.add(msElapsed);

        if (m_thrustDelayTimer.expired(false))
            m_velocity = m_heading * m_maxVelocity_km_s;

        if (m_blazerEnergyRequired.expired())
        {
            if (m_energy >= m_specialEnergyCost)
                consumeEnergy(m_specialEnergyCost);
            else
                setBlazerMode(false);
        }
    }
}

bool AndrosynthShipEntity::onPrimaryWeaponFired(Engine& engine)
{
    if (m_blazerMode)
        return false;

    const auto spawnLocation = m_position + (m_heading * m_radius_km);

    auto weaponEntity = std::make_shared<AndrosynthBubbleWeaponEntity>(shared_from_this(), spawnLocation, m_velocity, m_heading);
    engine.addEntity(weaponEntity);

    return true;
}

bool AndrosynthShipEntity::onSpecialWeaponFired(Engine& engine)
{
    setBlazerMode(true);
    return true;
}

void AndrosynthShipEntity::setBlazerMode(bool enabled)
{
    m_blazerMode = enabled;

    const auto props = MakeShipProperties();

    m_visualType = enabled ? ShipEntity::VisualType::AndrosynthBlazer : props.visualType;
    m_collisionDamage = enabled ? 1 : 0;
    m_maxVelocity_km_s = enabled ? props.maxVelocity_km_s * 1.5f : props.maxVelocity_km_s;
    m_generatesExhaust = !enabled;

    m_blazerEnergyRequired.reset(true);
}

ShipEntity::ShipProperties AndrosynthShipEntity::MakeShipProperties()
{
    ShipEntity::ShipProperties shipProps = {};

    shipProps.visualType = ShipEntity::VisualType::Androsynth;
    shipProps.mass_kg = 5.718e5;
    shipProps.engineForce_N = 8e3;
    shipProps.rotation_degPerSec = 100;
    shipProps.maxVelocity_km_s = 24;
    shipProps.maxHealth = 20;
    shipProps.maxEnergy = 24;
    shipProps.radius_km = 800;
    shipProps.primaryEnergyCost = 3;
    shipProps.specialEnergyCost = 1;
    shipProps.primaryFireRate_ms = 50;

    return shipProps;
}

// --

AndrosynthBubbleWeaponEntity::AndrosynthBubbleWeaponEntity(const std::shared_ptr<Entity>& parent, const Point& position, const Vector2d& velocity, const Vector2d& heading)
    : WeaponEntity(parent, MakeWeaponProperties(), position, velocity, heading)
    , m_bubbleRotator(0, 25)
{

}

WeaponEntity::WeaponProperties AndrosynthBubbleWeaponEntity::MakeWeaponProperties()
{
    WeaponEntity::WeaponProperties weaponProps = {};

    weaponProps.visualType = WeaponEntity::VisualType::AndrosynthBubble;
    weaponProps.collisionDamage = 2;
    weaponProps.homing = true;
    weaponProps.rotation_degPerSec = 30;
    weaponProps.maxAge_ms = 2000;

    return weaponProps;
}

void AndrosynthBubbleWeaponEntity::update(Engine& engine, uint32_t msElapsed)
{
    WeaponEntity::update(engine, msElapsed);

    m_bubbleRotator.add(msElapsed);
    if (m_bubbleRotator.expired())
        m_heading = ((NormalizedRandom() < .5f) ? RotationMatrix(-3) : RotationMatrix(3)) * m_heading;
}
