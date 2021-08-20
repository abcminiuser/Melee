#include "HumanShipEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    constexpr auto kMaxLaserTargetDistance = 10000;
}

HumanShipEntity::HumanShipEntity(const Point& position)
    : ShipEntity(MakeShipProperties(), position)
{

}

void HumanShipEntity::onEngineExhaustGenerated(Engine& engine)
{
    const auto spawnLocation = m_position + (-m_heading * (m_radius_km - 2));

    auto exhaustEntity = std::make_shared<ExhaustEntity>(shared_from_this(), ExhaustEntity::ExhaustProperties{}, spawnLocation, m_velocity);
    engine.addEntity(exhaustEntity, Engine::InsertionOrder::Bottom);
}

bool HumanShipEntity::onPrimaryWeaponFired(Engine& engine)
{
    const auto spawnLocation = m_position + (m_heading * m_radius_km);

    auto weaponEntity = std::make_shared<WeaponEntity>(shared_from_this(), MakePrimaryWeaponProperties(), spawnLocation, m_velocity, m_heading);
    engine.addEntity(weaponEntity);

    return true;
}

bool HumanShipEntity::onSpecialWeaponFired(Engine& engine)
{
    auto [distanceSquared, targetEntity] = engine.closestEntity(*this, [this](const Entity& other) { return (other.type() == Entity::Type::Ship || other.type() == Entity::Type::Asteroid) && &other != parentEntity().get(); });

    if (distanceSquared > (kMaxLaserTargetDistance * kMaxLaserTargetDistance))
        return false;

    // We need two entities, since this is a laser; a collidable one (which will damage instantly) and a non-collidable one (which will provide the visual feedback).

    auto weaponEntity = std::make_shared<WeaponEntity>(shared_from_this(), MakeSpecialWeaponProperties(true), targetEntity->position());
    engine.addEntity(weaponEntity);

    auto weaponEntity2 = std::make_shared<WeaponEntity>(shared_from_this(), MakeSpecialWeaponProperties(false), targetEntity->position());
    engine.addEntity(weaponEntity2);

    return true;
}

ShipEntity::ShipProperties HumanShipEntity::MakeShipProperties()
{
    ShipEntity::ShipProperties shipProps = {};

    shipProps.visualType = ShipEntity::VisualType::Human;
    shipProps.mass_kg = 9.718e5;
    shipProps.engineForce_N = 8e3;
    shipProps.rotation_degPerSec = 100;
    shipProps.maxVelocity_km_s = 24;
    shipProps.maxHealth = 18;
    shipProps.maxEnergy = 18;
    shipProps.radius_km = 1000;
    shipProps.primaryEnergyCost = 9;
    shipProps.specialEnergyCost = 4;
    shipProps.primaryFireRate_ms = 3000;
    shipProps.specialFireRate_ms = 300;

    return shipProps;
}

WeaponEntity::WeaponProperties HumanShipEntity::MakePrimaryWeaponProperties()
{
    WeaponEntity::WeaponProperties weaponProps = {};

    weaponProps.visualType = WeaponEntity::VisualType::HumanMissile;
    weaponProps.damage = 4;
    weaponProps.homing = true;
    weaponProps.rotation_degPerSec = 120;
    weaponProps.maxAge_ms = 8000;

    return weaponProps;
}

WeaponEntity::WeaponProperties HumanShipEntity::MakeSpecialWeaponProperties(bool collidable)
{
    WeaponEntity::WeaponProperties weaponProps = {};

    weaponProps.visualType = WeaponEntity::VisualType::HumanLaser;
    weaponProps.damage = 1;
    weaponProps.maxAge_ms = 100;
    weaponProps.collidable = collidable;

    return weaponProps;
}
