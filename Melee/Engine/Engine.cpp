#include "Engine/Engine.hpp"

using namespace Melee;

Engine::Engine()
{

}

void Engine::update(uint32_t msElapsed)
{
	for (const auto& entity : m_entities)
		entity->update(*this, msElapsed);

	const auto totalEntities = m_entities.size();
	if (totalEntities >= 2)
	{
		for (auto e1 = m_entities.begin(); e1 != m_entities.end(); e1++)
		{
			for (auto e2 = std::next(e1); e2 != m_entities.end(); e2++)
			{
				const auto& entity1 = *e1;
				const auto& entity2 = *e2;

				if (!m_collisionCallback || !m_collisionCallback(entity1, entity2))
					continue;

				entity1->collide(*this, *entity2);
				entity2->collide(*this, *entity1);
			}
		}
	}
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
