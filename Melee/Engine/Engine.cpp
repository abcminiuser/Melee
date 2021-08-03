#include "Engine/Engine.hpp"

#include <algorithm>

using namespace Melee;

namespace
{
    constexpr auto kMaxUpdateTimestepMs = 8;
}

Engine::Engine(float playfieldSize)
    : m_playfieldSize(playfieldSize)
{

}

void Engine::update(uint32_t msElapsed)
{
    m_updateMsElapsed += msElapsed;

    while (m_updateMsElapsed >= kMaxUpdateTimestepMs)
    {
        handleDeferredEntityAddRemove();
        checkForEntityCollisions();
        updatePlayersBoundingBox();

        for (const auto& entity : m_entities)
            entity->update(*this, kMaxUpdateTimestepMs);

        for (auto* observer : m_observers)
            observer->updated(*this, kMaxUpdateTimestepMs);

        m_updateMsElapsed -= kMaxUpdateTimestepMs;
    }
}

void Engine::addEntity(const std::shared_ptr<Entity>& entity, InsertionOrder zOrder) noexcept
{
    if (zOrder == InsertionOrder::Top)
        m_entitiesToAddTop.emplace_back(entity);
    else
        m_entitiesToAddBottom.emplace_back(entity);
}

void Engine::removeEntity(const std::shared_ptr<Entity>& entity) noexcept
{
    m_entitiesToRemove.emplace_back(entity);
}

void Engine::handleDeferredEntityAddRemove()
{
    if (!m_entitiesToAddBottom.empty())
    {
        for (const auto& addBottomEntity : m_entitiesToAddBottom)
        {
            m_entities.push_front(addBottomEntity);

            for (auto* observer : m_observers)
                observer->entityAdded(*this, addBottomEntity);
        }

        m_entitiesToAddBottom.clear();
    }

    if (!m_entitiesToAddTop.empty())
    {
        for (const auto& addTopEntity : m_entitiesToAddTop)
        {
            m_entities.push_back(addTopEntity);

            for (auto* observer : m_observers)
                observer->entityAdded(*this, addTopEntity);
        }

        m_entitiesToAddTop.clear();
    }

    while (!m_entitiesToRemove.empty())
    {
        EntityList deferredEntitiesToRemove;

        for (const auto& removeEntity : m_entitiesToRemove)
        {
            // Queue up any entities owned by this entity for deferred removal.
            for (const auto& entity : m_entities)
            {
                if (entity->parentEntity() == removeEntity)
                    deferredEntitiesToRemove.push_back(entity);
            }

            m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [&](const auto& e) { return e == removeEntity; }), m_entities.end());

            for (auto* observer : m_observers)
                observer->entityRemoved(*this, removeEntity);
        }

        m_entitiesToRemove = std::move(deferredEntitiesToRemove);
    }
}

bool Engine::checkCollison(const std::shared_ptr<Entity>& entity1, const std::shared_ptr<Entity>& entity2) const noexcept
{
	if (! entity1->properties().collidable || !entity2->properties().collidable)
		return false;

	const auto distanceBetweenEntitiesSquared = (entity1->position() - entity2->position()).lengthSquared();
	const auto minCollisionRadius = entity1->properties().radius_km + entity2->properties().radius_km;

	// Crude radius check
	return (distanceBetweenEntitiesSquared < (minCollisionRadius * minCollisionRadius));
}

void Engine::checkForEntityCollisions()
{
    const auto totalEntities = m_entities.size();
    if (totalEntities < 2)
        return;

    for (auto e1 = m_entities.begin(); e1 != m_entities.end(); e1++)
    {
        const auto& entity1 = *e1;

        if (!entity1->properties().collidable)
            continue;

        for (auto e2 = std::next(e1); e2 != m_entities.end(); e2++)
        {
            const auto& entity2 = *e2;

            if (!checkCollison(entity1, entity2))
            	continue;

			// We need to copy out the pre-collision kinematic states before we start processing the collisions.
			const auto e1Precollision = entity1->preCollisionState();
			const auto e2Precollision = entity2->preCollisionState();

			// Now process the collision on each object.
			entity1->collide(*this, *entity2, e2Precollision);
			entity2->collide(*this, *entity1, e1Precollision);

			for (auto* observer : m_observers)
				observer->collision(*this, entity1, entity2);
        }
    }
}

void Engine::updatePlayersBoundingBox()
{
    float minX = m_playfieldSize;
    float minY = m_playfieldSize;
    float maxX = 0;
    float maxY = 0;

    for (const auto& entity : m_entities)
    {
        if (entity->type() != Entity::Type::Player)
            continue;

        const auto pos = entity->position();
        const auto radius = entity->properties().radius_km;

        minX = std::min(minX, pos.x - radius);
        maxX = std::max(maxX, pos.x + radius);
        minY = std::min(minY, pos.y - radius);
        maxY = std::max(maxY, pos.y + radius);
    }

    m_playersBoundingBox = Rectangle{ { minX, minY}, {maxX - minX, maxY - minY } };
}
