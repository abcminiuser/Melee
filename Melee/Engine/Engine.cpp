#include "Engine/Engine.hpp"

using namespace Melee;

Engine::Engine()
{
	auto player1 = std::make_shared<PlayerEntity>(0, Point{ 200 * 1000, 200 * 1000 });
	m_entities.emplace_back(player1);

	auto player2 = std::make_shared<PlayerEntity>(1, Point{ 100, 100 });
	m_entities.emplace_back(player2);
}

void Engine::update(uint32_t msElapsed)
{
	for (const auto& entity : m_entities)
		entity->update(msElapsed);
}
