#pragma once

#include <vector>
#include <memory>

#include "EngineTypes.hpp"

#include "Entity.hpp"
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
		std::vector<std::shared_ptr<Entity>> m_entities;
	};
}
