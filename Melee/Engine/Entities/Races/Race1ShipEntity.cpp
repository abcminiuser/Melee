#include "Race1ShipEntity.hpp"

#include "Engine/Engine.hpp"

using namespace Melee;


Race1ShipEntity::Race1ShipEntity(const Point& position)
    : ShipEntity(MakeShipProperties(), position)
{

}

void Race1ShipEntity::onPrimaryWeaponFired(Engine& engine)
{
	const auto spawnLocation = m_position + (m_heading * m_shipProperties.radius_km);

    auto weaponEntity = std::make_shared<WeaponEntity>(shared_from_this(), MakePrimaryWeaponProperties(), spawnLocation, m_velocity, m_heading);
    engine.addEntity(weaponEntity);
}

ShipEntity::ShipProperties Race1ShipEntity::MakeShipProperties()
{
    ShipEntity::ShipProperties shipProps = {};

    shipProps.visualType = ShipEntity::VisualType::Race1Ship;
    shipProps.mass_kg = 9.718e5;
    shipProps.engineForce_N = 8e3;
    shipProps.rotation_degPerSec = 100;
    shipProps.maxVelocity_km_s = 40;
    shipProps.maxHealth = 2;
    shipProps.maxEnergy = 10;
    shipProps.radius_km = 1000;

    return shipProps;
}

WeaponEntity::WeaponProperties Race1ShipEntity::MakePrimaryWeaponProperties()
{
	WeaponEntity::WeaponProperties weaponProps = {};

	weaponProps.visualType = WeaponEntity::VisualType::Race1Missile;

	return weaponProps;
}
