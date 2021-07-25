#pragma once

#include "EngineTypes.hpp"

#include "Entity.hpp"
#include "Entities/AsteroidEntity.hpp"
#include "Entities/ExhaustEntity.hpp"
#include "Entities/PlanetEntity.hpp"
#include "Entities/PlayerEntity.hpp"

#include <functional>
#include <unordered_map>

namespace Melee
{
	class Engine final
	{
	public:
		using CollisionCallback = std::function<bool(const std::shared_ptr<Entity>&, const std::shared_ptr<Entity>&)>;

		explicit	Engine();
					~Engine() = default;

		auto&		getEntities()										{ return m_entities; }
		auto&		getEntities(Entity::Type type)						{ return m_entifiesForType[type]; }

		void		setCollisionCallback(CollisionCallback&& callback)	{ m_collisionCallback = callback;  }

		void		update(uint32_t msElapsed);

		void		addEntity(const std::shared_ptr<Entity>& entity);
		void		removeEntity(const std::shared_ptr<Entity>& entity);

	private:
		void		handleDeferredEntityAddRemove();
		void		checkForEntityCollisions();

	private:
		using EntityMap = std::unordered_map<Entity::Type, EntityList>;

		EntityList			m_entities;
		EntityMap			m_entifiesForType;

		EntityList			m_entitiesToAdd;
		EntityList			m_entitiesToRemove;

		CollisionCallback	m_collisionCallback;
	};
}
