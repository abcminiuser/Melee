#pragma once

#include "Engine/Entity.hpp"

namespace Melee
{
	class PlanetEntity : public Entity
	{
	public:
		struct PlanetProperties : public Properties
		{
			
		};

	public:
		explicit				PlanetEntity(const PlanetProperties& properties, const Point& pos);
		virtual					~PlanetEntity() = default;

		const auto& 			properties() const { return m_planetProperties; }

	// Entity i/f:
	public:
		void					update(Engine& engine, uint32_t msElapsed) override;

	private:
		const PlanetProperties	m_planetProperties;
	};
}
