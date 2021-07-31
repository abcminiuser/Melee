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

#include <forward_list>
#include <unordered_map>

namespace Melee
{
    class Engine final
    {
    public:
        class Observer;

        explicit    Engine(float playfieldSize);
                    ~Engine() = default;

        void        addObserver(Observer* observer)                     { m_observers.push_front(observer); }
        void        removeObserver(Observer* observer)                  { m_observers.remove(observer); }

        auto&       getEntities()                                       { return m_entities; }
        auto&       getEntities(Entity::Type type)                      { return m_entitiesForType[type]; }
        auto        getEntities(const std::shared_ptr<Entity>& parent)
        {
            if (const auto foundList = m_entitiesForParent.find(parent); foundList != m_entitiesForParent.end())
                return foundList->second;

            return EntityList{};
        }

        float       getPlayfieldSize() const                            { return m_playfieldSize; }
        Rectangle   getPlayersBoundingBox();

        void        update(uint32_t msElapsed);

        void        addEntity(const std::shared_ptr<Entity>& entity) noexcept;
        void        removeEntity(const std::shared_ptr<Entity>& entity) noexcept;

    private:
        void        handleDeferredEntityAddRemove() noexcept;
        void        checkForEntityCollisions();

    private:
        template <typename K>
        using EntityMap = std::unordered_map<K, EntityList>;

        const float                         m_playfieldSize;

        std::forward_list<Observer*>        m_observers;

        EntityList                          m_entities;
        EntityMap<Entity::Type>             m_entitiesForType;
        EntityMap<std::shared_ptr<Entity>>  m_entitiesForParent;

        EntityList                          m_entitiesToAdd;
        EntityList                          m_entitiesToRemove;

        uint32_t                            m_updateMsElapsed = 0;
    };

    class Engine::Observer
    {
    public:
        virtual         ~Observer() = default;

        virtual void    entityAdded(Engine& engine, const std::shared_ptr<Entity>& entity)      {}
        virtual void    entityRemoved(Engine& engine, const std::shared_ptr<Entity>& entity)    {}
        virtual void    collision(Engine& engine, const std::shared_ptr<Entity>& entity1, const std::shared_ptr<Entity>& entity2)   {};
        virtual void    updated(Engine& engine, uint32_t msElapsed)                             {}
    };
}
