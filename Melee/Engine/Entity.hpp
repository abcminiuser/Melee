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

		struct Properties
		{
			float		mass_kg = 0;
			float		radius_km = 0;
			uint32_t	maxVelocity = 0;
		};

	public:
		explicit		Entity(Type type, const Properties& properties, const Point& pos);
		virtual			~Entity() = default;

		virtual void	update(Engine& engine, uint32_t msElapsed);
		virtual void	collide(Engine& engine, Entity& otherEntity);

		Type			type() const { return m_type; }
		const auto&		properties() const { return m_properties; }

		Point			position() const { return m_position; }
		Vector2d		heading() const { return m_heading; }

		void			applyExternalForce(const Vector2d& forceVector);

	protected:
		const Type			m_type;
		const Properties	m_properties;

		float				m_maxVelocitySquared = 0;

		Point				m_position;
		Vector2d			m_heading = { 1, 0 };

		Vector2d			m_velocity;
		Vector2d			m_acceleration;

		Vector2d			m_externalAcceleration;
	};
}
