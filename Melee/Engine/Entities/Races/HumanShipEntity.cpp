#include "HumanShipEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;

namespace
{
    constexpr auto kMaxMines = 5;
}

HumanShipEntity::HumanShipEntity(const Point& position)
    : ShipEntity(MakeShipProperties(), position)
{

}

void HumanShipEntity::onEngineExhaustGenerated(Engine& engine)
{
    const auto spawnLocation = m_position + (-m_heading * m_radius_km);

    auto exhaustEntity = std::make_shared<ExhaustEntity>(shared_from_this(), ExhaustEntity::ExhaustProperties{}, spawnLocation, m_velocity);
    engine.addEntity(exhaustEntity, Engine::InsertionOrder::Bottom);
}

void HumanShipEntity::onPrimaryWeaponFired(Engine& engine)
{
	const auto spawnLocation = m_position + (m_heading * m_radius_km);

    auto weaponEntity = std::make_shared<WeaponEntity>(shared_from_this(), MakePrimaryWeaponProperties(), spawnLocation, m_velocity, m_heading);
    engine.addEntity(weaponEntity);
}

void HumanShipEntity::onSpecialWeaponFired(Engine& engine)
{
	// TODO:
}

ShipEntity::ShipProperties HumanShipEntity::MakeShipProperties()
{
    ShipEntity::ShipProperties shipProps = {};

    shipProps.visualType = ShipEntity::VisualType::Human;
    shipProps.mass_kg = 9.718e5;
    shipProps.engineForce_N = 8e3;
    shipProps.rotation_degPerSec = 100;
    shipProps.maxVelocity_km_s = 40;
    shipProps.maxHealth = 2;
    shipProps.maxEnergy = 10;
    shipProps.radius_km = 1000;

    return shipProps;
}

WeaponEntity::WeaponProperties HumanShipEntity::MakePrimaryWeaponProperties()
{
    WeaponEntity::WeaponProperties weaponProps = {};

    weaponProps.visualType = WeaponEntity::VisualType::HumanMissile;
    weaponProps.homing = true;
    weaponProps.rotation_degPerSec = 120;
    weaponProps.damage = 1;

    return weaponProps;
}

WeaponEntity::WeaponProperties HumanShipEntity::MakeSpecialWeaponProperties()
{
    WeaponEntity::WeaponProperties weaponProps = {};

    // TODO:

    return weaponProps;
}
