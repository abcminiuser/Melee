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

        enum class InsertionOrder
        {
            Bottom,
            Top,
        };

        explicit    Engine(float playfieldSize);
                    ~Engine() = default;

        void        addObserver(Observer* observer)                     { m_observers.push_front(observer); }
        void        removeObserver(Observer* observer)                  { m_observers.remove(observer); }

        auto&       getEntities()                                       { return m_entities; }
        auto&       getEntities(Entity::Type type)                      { return m_entitiesForType[type]; }
        auto        getEntities(const std::shared_ptr<Entity>& parent)
        {
            const auto foundList = m_entitiesForParent.find(parent);
            if (foundList != m_entitiesForParent.end())
                return foundList->second;

            return EntityList{};
        }

        float       getPlayfieldSize() const                            { return m_playfieldSize; }
        Rectangle   getPlayersBoundingBox()                             { return m_playersBoundingBox; }

        void        update(uint32_t msElapsed);

        void        addEntity(const std::shared_ptr<Entity>& entity, InsertionOrder zOrder = InsertionOrder::Top) noexcept;
        void        removeEntity(const std::shared_ptr<Entity>& entity) noexcept;

        bool		checkCollison(const std::shared_ptr<Entity>& entity1, const std::shared_ptr<Entity>& entity2) const noexcept;

    private:
        void        handleDeferredEntityAddRemove();
        void        checkForEntityCollisions();
        void        updatePlayersBoundingBox();

    private:
        template <typename K>
        using EntityMap = std::unordered_map<K, EntityList>;

        const float                         m_playfieldSize;

        std::forward_list<Observer*>        m_observers;

        EntityList                          m_entities;
        EntityMap<Entity::Type>             m_entitiesForType;
        EntityMap<std::shared_ptr<Entity>>  m_entitiesForParent;

        EntityList                          m_entitiesToAddTop;
        EntityList                          m_entitiesToAddBottom;
        EntityList                          m_entitiesToRemove;

        Rectangle                           m_playersBoundingBox;

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
