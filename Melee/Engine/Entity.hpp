#pragma once

#include "EngineTypes.hpp"

namespace Melee
{
	class Entity
	{
	public:
		enum class Type
		{
			Player,
			Planet,
			Asteroid,
			Projectile,
		};

	public:
		explicit		Entity(Type type, const Point& pos);
		virtual			~Entity() = default;

		virtual void	update(uint32_t msElapsed);

		Type			type() const		{ return m_type; }
		Point			position() const	{ return m_position; }
		Vector2d		heading() const		{ return m_heading; }

		void			setMaxVelocity(uint32_t velocity);

	protected:
		const Type	m_type;

		Point		m_position;
		Vector2d	m_heading;

		Vector2d    m_velocity;
		Vector2d    m_acceleration;

		uint32_t	m_maxVelocitySquared = 0;
	};
}
