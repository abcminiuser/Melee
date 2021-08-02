#include "Engine/Engine.hpp"

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
        for (const auto& entity : m_entitiesToAddBottom)
        {
            m_entities.push_front(entity);
            m_entitiesForType[entity->type()].push_front(entity);

            // Add this entity to the list of entities owned by its parent.
            if (const auto& parentEntity = entity->parentEntity())
                m_entitiesForParent[parentEntity].push_front(entity);

            for (auto* observer : m_observers)
                observer->entityAdded(*this, entity);
        }

        m_entitiesToAddBottom.clear();
    }

    if (!m_entitiesToAddTop.empty())
    {
        for (const auto& entity : m_entitiesToAddTop)
        {
            m_entities.push_back(entity);
            m_entitiesForType[entity->type()].push_back(entity);

            // Add this entity to the list of entities owned by its parent.
            if (const auto& parentEntity = entity->parentEntity())
                m_entitiesForParent[parentEntity].push_back(entity);

            for (auto* observer : m_observers)
                observer->entityAdded(*this, entity);
        }

        m_entitiesToAddTop.clear();
    }

    if (!m_entitiesToRemove.empty())
    {
        static constexpr auto RemoveFromContainer =
            [](auto& container, const auto value)
            {
                container.erase(std::remove_if(container.begin(), container.end(), [&](const auto& e) { return e == value; }), container.end());
            };

        EntityList deferredEntitiesToRemove;

        for (const auto& entity : m_entitiesToRemove)
        {
            RemoveFromContainer(m_entities, entity);
            RemoveFromContainer(m_entitiesForType[entity->type()], entity);

            // Remove this entity from the list of entities owned by its parent.
            if (const auto& parentEntity = entity->parentEntity())
                RemoveFromContainer(m_entitiesForParent[parentEntity], entity);

            // Remove all entities owned by this entity (on the next update loop).
            deferredEntitiesToRemove.insert(deferredEntitiesToRemove.end(), m_entitiesForParent[entity].begin(), m_entitiesForParent[entity].end());

            // Also remove the list of entities owned by this entity, so we reclaim some memory.
            m_entitiesForParent.erase(entity);

            for (auto* observer : m_observers)
                observer->entityRemoved(*this, entity);
        }

        m_entitiesToRemove = std::move(deferredEntitiesToRemove);
    }
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

            if (!entity2->properties().collidable)
                continue;

            const auto distanceBetweenEntitiesSquared = (entity1->position() - entity2->position()).lengthSquared();
            const auto minCollisionRadius = entity1->properties().radius_km + entity2->properties().radius_km;

            // Crude radius check, which we can do in the engine without the renderer.
            if (distanceBetweenEntitiesSquared > (minCollisionRadius * minCollisionRadius))
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

    for (const auto& entity : m_entitiesForType[Entity::Type::Player])
    {
        const auto pos = entity->position();
        const auto radius = entity->properties().radius_km;

        minX = std::min(minX, pos.x - radius);
        maxX = std::max(maxX, pos.x + radius);
        minY = std::min(minY, pos.y - radius);
        maxY = std::max(maxY, pos.y + radius);
    }

    m_playersBoundingBox = Rectangle{ { minX, minY}, {maxX - minX, maxY - minY } };
}
