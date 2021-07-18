#include "Engine/Engine.hpp"

using namespace Melee;

Engine::Engine()
{
	PlayerEntity::PlayerProperties playerProps = {};
	playerProps.mass_kg = 90.718e6;
	playerProps.engineForce_N = 2e9;
	playerProps.maxVelocity = 0;
	playerProps.rotation_degPerSec = 100;
	playerProps.maxVelocity = 1000;

	auto player1 = std::make_shared<PlayerEntity>(0, playerProps, Point{ 200 * 1000, 200 * 1000 });
	m_entities.emplace_back(player1);

	auto player2 = std::make_shared<PlayerEntity>(1, playerProps, Point{ 100, 100 });
	m_entities.emplace_back(player2);

	PlanetEntity::PlanetProperties planetProps = {};
	planetProps.mass_kg = 5.9736e24 / 1000;
	planetProps.radius_km = 6371;

	auto planet = std::make_shared<PlanetEntity>(planetProps, Point{ 800 * 1000, 300 * 1000 });
	m_entities.emplace_back(planet);
}

void Engine::update(uint32_t msElapsed)
{
	for (const auto& entity : m_entities)
		entity->update(m_entities, msElapsed);
}
