#pragma once

#include "EngineTypes.hpp"

#include "Util/EngineUtils.hpp"

#include "Entity.hpp"
#include "Entities/AsteroidEntity.hpp"
#include "Entities/ExhaustEntity.hpp"
#include "Entities/ExplosionEntity.hpp"
#include "Entities/PlanetEntity.hpp"
#include "Entities/PlayerEntity.hpp"
#include "Entities/ProjectileEntity.hpp"

#include <functional>
#include <unordered_map>

namespace Melee
{
    class Engine final
    {
    public:
        using CollisionCallback = std::function<bool(const std::shared_ptr<Entity>&, const std::shared_ptr<Entity>&)>;

        explicit    Engine(const uint32_t playfieldSize);
                    ~Engine() = default;

        auto&       getEntities()                                       { return m_entities; }
        auto&       getEntities(Entity::Type type)                      { return m_entitiesForType[type]; }
        auto        getEntities(const std::shared_ptr<Entity>& parent)
        {
            if (const auto foundList = m_entitiesForParent.find(parent); foundList != m_entitiesForParent.end())
                return foundList->second;

            return EntityList{};
        }

        uint32_t    getPlayfieldSize() const							{ return m_playfieldSize; }
        Rectangle   getPlayersBoundingBox();

        void        setCollisionCallback(CollisionCallback&& callback)  { m_collisionCallback = callback;  }

        void        update(uint32_t msElapsed);

        void        addEntity(const std::shared_ptr<Entity>& entity) noexcept;
        void        removeEntity(const std::shared_ptr<Entity>& entity) noexcept;

    private:
        void        handleDeferredEntityAddRemove() noexcept;
        void        checkForEntityCollisions();

    private:
        template <typename K>
        using EntityMap = std::unordered_map<K, EntityList>;

        const uint32_t		                m_playfieldSize;

        EntityList                          m_entities;
        EntityMap<Entity::Type>             m_entitiesForType;
        EntityMap<std::shared_ptr<Entity>>  m_entitiesForParent;

        EntityList                          m_entitiesToAdd;
        EntityList                          m_entitiesToRemove;

        CollisionCallback                   m_collisionCallback;

        uint32_t                            m_updateMsElapsed = 0;
    };
}
