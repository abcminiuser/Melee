#pragma once

#include "EngineTypes.hpp"

#include "Util/EngineUtils.hpp"

#include "Entity.hpp"
#include "Entities/AsteroidEntity.hpp"
#include "Entities/ExhaustEntity.hpp"
#include "Entities/ExplosionEntity.hpp"
#include "Entities/PlanetEntity.hpp"
#include "Entities/ShipEntity.hpp"
#include "Entities/WeaponEntity.hpp"

#include "Entities/Races/Androsynth.hpp"
#include "Entities/Races/Arilou.hpp"
#include "Entities/Races/Human.hpp"

#include <deque>
#include <forward_list>
#include <functional>
#include <utility>
#include <vector>

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

        float       getPlayfieldSize() const noexcept                   { return m_playfieldSize; }
        Rectangle   getShipsBoundingBox() const noexcept                { return m_shipsBoundingBox; }

        void        update(uint32_t msElapsed);

        void        addEntity(const std::shared_ptr<Entity>& entity, InsertionOrder zOrder = InsertionOrder::Top) noexcept;
        void        removeEntity(const std::shared_ptr<Entity>& entity) noexcept;

        bool	    checkCollison(const std::shared_ptr<Entity>& entity1, const std::shared_ptr<Entity>& entity2) const noexcept;
        std::pair<float, const std::shared_ptr<Entity>> closestEntity(Entity& originEntity, const std::function<bool(const Entity&)> filter);

    private:
        void        handleDeferredEntityAddRemove();
        void        checkForEntityCollisions();
        void        updateShipsBoundingBox();

    private:
		using EntityAddList = std::vector<std::pair<std::shared_ptr<Entity>, InsertionOrder>>;
  		using EntityRemoveList = std::vector<std::shared_ptr<Entity>>;

        const float                         	m_playfieldSize;

        std::forward_list<Observer*>        	m_observers;

        std::deque<std::shared_ptr<Entity>> 	m_entities;

        EntityAddList							m_entitiesToAdd;
        EntityRemoveList						m_entitiesToRemove;

        Rectangle                           	m_shipsBoundingBox;

        uint32_t                            	m_updateMsElapsed = 0;
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
