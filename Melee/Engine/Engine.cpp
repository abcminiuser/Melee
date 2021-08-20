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
        updateShipsBoundingBox();

        for (const auto& entity : m_entities)
            entity->update(*this, kMaxUpdateTimestepMs);

        for (auto* observer : m_observers)
            observer->updated(*this, kMaxUpdateTimestepMs);

        m_updateMsElapsed -= kMaxUpdateTimestepMs;
    }

    handleDeferredEntityAddRemove();
}

void Engine::addEntity(const std::shared_ptr<Entity>& entity, InsertionOrder zOrder) noexcept
{
	m_entitiesToAdd.emplace_back(std::make_pair(entity, zOrder));
}

void Engine::removeEntity(const std::shared_ptr<Entity>& entity) noexcept
{
    m_entitiesToRemove.emplace_back(entity);
}

void Engine::handleDeferredEntityAddRemove()
{
    if (!m_entitiesToAdd.empty())
    {
        for (const auto& [addEntity, zOrder] : m_entitiesToAdd)
        {
        	if (zOrder == InsertionOrder::Bottom)
            	m_entities.push_front(addEntity);
            else
				m_entities.push_back(addEntity);

            for (auto* observer : m_observers)
                observer->entityAdded(*this, addEntity);
        }

        m_entitiesToAdd.clear();
    }

    while (!m_entitiesToRemove.empty())
    {
        EntityRemoveList deferredEntitiesToRemove;

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
	if (! entity1->collidable() || !entity2->collidable())
		return false;

	const auto distanceBetweenEntitiesSquared = (entity1->position() - entity2->position()).lengthSquared();
	const auto minCollisionRadius = entity1->radius() + entity2->radius();

	// Crude radius check
	return (distanceBetweenEntitiesSquared < (minCollisionRadius * minCollisionRadius));
}

std::pair<float, const std::shared_ptr<Entity>> Engine::closestEntity(Entity& originEntity, const std::function<bool(const Entity&)> filter)
{
    float minDistanceSquared = std::numeric_limits<float>::max();

    std::shared_ptr<Entity> closestEntity;

    for (const auto& entity : m_entities)
    {
        if (entity.get() == &originEntity)
            continue;

        if (filter && !filter(*entity))
            continue;

        const auto distanceToTargetSquared = (entity->position() - originEntity.position()).lengthSquared();
        if (distanceToTargetSquared < minDistanceSquared)
        {
            minDistanceSquared = distanceToTargetSquared;
            closestEntity = entity;
        }
    }

    return std::make_pair(minDistanceSquared, closestEntity);
}

void Engine::checkForEntityCollisions()
{
    const auto totalEntities = m_entities.size();
    if (totalEntities < 2)
        return;

    for (auto e1 = m_entities.begin(); e1 != m_entities.end(); e1++)
    {
        const auto& entity1 = *e1;

        if (!entity1->collidable())
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
			entity1->collide(*this, entity2, e2Precollision);
			entity2->collide(*this, entity1, e1Precollision);

			for (auto* observer : m_observers)
				observer->collision(*this, entity1, entity2);
        }
    }
}

void Engine::updateShipsBoundingBox()
{
    float minX = m_playfieldSize;
    float minY = m_playfieldSize;
    float maxX = 0;
    float maxY = 0;

    for (const auto& entity : m_entities)
    {
        if (entity->type() != Entity::Type::Ship)
            continue;

        const auto pos = entity->position();
        const auto radius = entity->radius();

        minX = std::min(minX, pos.x - radius);
        maxX = std::max(maxX, pos.x + radius);
        minY = std::min(minY, pos.y - radius);
        maxY = std::max(maxY, pos.y + radius);
    }

    m_shipsBoundingBox = Rectangle{ { minX, minY}, {maxX - minX, maxY - minY } };
}
