#include "Engine/Engine.hpp"

using namespace Melee;

Engine::Engine()
{

}

void Engine::update(uint32_t msElapsed)
{
	// Check for collisions between entities.
	const auto totalEntities = m_entities.size();
	if (totalEntities >= 2)
	{
		for (auto e1 = m_entities.begin(); e1 != m_entities.end(); e1++)
		{
			for (auto e2 = std::next(e1); e2 != m_entities.end(); e2++)
			{
				const auto& entity1 = *e1;
				const auto& entity2 = *e2;

				const auto distanceBetweenEntitiesSquared = (entity1->position() - entity2->position()).lengthSquared();
				const auto minCollisionRadius = entity1->properties().radius_km + entity2->properties().radius_km;

				// Crude radius check first, which we can do in the engine without the renderer.
				if (distanceBetweenEntitiesSquared > (minCollisionRadius * minCollisionRadius))
					continue;

				// Ask the renderer to check for a more accurate collision based on the current assets.
				if (!m_collisionCallback || !m_collisionCallback(entity1, entity2))
					continue;

				entity1->collide(*this, *entity2);
				entity2->collide(*this, *entity1);
			}
		}
	}

	// Update entity states.
	for (const auto& entity : m_entities)
		entity->update(*this, msElapsed);
}

void Engine::addEntity(const std::shared_ptr<Entity>& entity)
{
	m_entities.emplace_front(entity);
	m_entifiesForType[entity->type()].emplace_front(entity);
}

void Engine::removeEntity(const std::shared_ptr<Entity>& entity)
{
	m_entities.remove(entity);
	m_entifiesForType[entity->type()].remove(entity);
}
