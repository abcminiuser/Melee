#pragma once

#include "Entity.hpp"

namespace Melee
{
	class PlanetEntity : public Entity
	{
	public:
		struct PlanetProperties : Properties
		{
			float	radius_km;
		};

	public:
		explicit	PlanetEntity(const PlanetProperties& properties, const Point& pos);
		virtual		~PlanetEntity() = default;

		float		radius() const { return m_planetProperties.radius_km; }

	private:
		const PlanetProperties	m_planetProperties;
	};
}
