#pragma once

#include "EngineTypes.hpp"

#include "Entity.hpp"
#include "PlanetEntity.hpp"
#include "PlayerEntity.hpp"

namespace Melee
{
	class Engine final
	{
	public:
		explicit	Engine();
					~Engine() = default;

		auto&		getEntities() { return m_entities; }

		void		update(uint32_t msElapsed);

	private:
		EntityList	m_entities;
	};
}
