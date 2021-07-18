#include "Engine/Engine.hpp"

using namespace Melee;

Engine::Engine()
{
	PlayerEntity::PlayerProperties playerProps = {};
	playerProps.mass_t = 90718; // Cruise ship average displacement, close enough?
	playerProps.engineForce_GN = 2000;
	playerProps.maxVelocity = 0;
	playerProps.rotation_degPerSec = 100;
	playerProps.maxVelocity = 1000;

	auto player1 = std::make_shared<PlayerEntity>(0, playerProps, Point{ 200 * 1000, 200 * 1000 });
	m_entities.emplace_back(player1);

	auto player2 = std::make_shared<PlayerEntity>(1, playerProps, Point{ 100, 100 });
	m_entities.emplace_back(player2);
}

void Engine::update(uint32_t msElapsed)
{
	for (const auto& entity : m_entities)
		entity->update(msElapsed);
}
